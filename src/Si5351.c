#include "main.h"
#include "Si5351.h"

static uint32_t si5351_XTAL_FREQ = SI5351_XTAL_FREQ;
struct Si5351Status dev_status;
struct Si5351IntStatus dev_int_status;

/******************************/
/* Suggested private functions */
/******************************/

static void rational_best_approximation(
    uint64_t given_numerator, uint64_t given_denominator,
    uint32_t max_numerator, uint32_t max_denominator,
    uint32_t *best_numerator, uint32_t *best_denominator);
static void set_multisynth_alt(uint32_t freq, enum si5351_clock clk);
static uint8_t si5351_write_bulk(uint8_t, uint8_t, uint8_t *);
static uint8_t si5351_write(uint8_t, uint8_t);
static uint8_t si5351_read(uint8_t, uint8_t *);
static void si5351_set_clk_control(enum si5351_clock, enum si5351_pll, int isIntegerMode, enum si5351_drive drive);
static void si5351_set_ms(uint32_t a, uint32_t b, uint32_t c, uint8_t rdiv, enum si5351_clock clk);

/******************************/
/* Suggested public functions */
/******************************/

/*
 * si5351_init(void)
 *
 * Call this to initialize I2C communications and get the
 * Si5351 ready for use.
 */
void si5351_init(void)
{
    //si5351_XTAL_FREQ = (uint32_t)((int)SI5351_XTAL_FREQ + BKUP_LoadXtalCorr());
    si5351_XTAL_FREQ = SdrStr.SdrSetup.DDS_mck[3];
    i2c_init();

    // Set crystal load capacitance
    si5351_write(SI5351_CRYSTAL_LOAD, SI5351_CRYSTAL_LOAD_8PF);

    //Disable spread spectrum (value after reset is unknown)
    si5351_write(SI5351_SSC_PARAM0, 0);

    //Disable fanout (initial value is unknown)
    si5351_write(SI5351_FANOUT_ENABLE, 0);
}

/*
 * si5351_set_freq(uint32_t freq, enum si5351_clock output)
 *
 * Sets the clock frequency of the specified CLK output
 *
 * freq - Output frequency in Hz
 * clk - Clock output
 *   (use the si5351_clock enum)
 */

void si5351_set_freq(uint32_t freq, enum si5351_clock clk)
{
    set_multisynth_alt(freq, clk);
}

/*
 * si5351_clock_enable(enum si5351_clock clk, uint8_t enable)
 *
 * Enable or disable a chosen clock
 * clk - Clock output
 *   (use the si5351_clock enum)
 * enable - Set to 1 to enable, 0 to disable
 */
void si5351_clock_enable(enum si5351_clock clk, uint8_t enable)
{
    uint8_t reg_val;

    /*if(si5351_read(SI5351_OUTPUT_ENABLE_CTRL, &reg_val) != 0)
    {
        return;
    }*/

    if(enable == 1)
    {
        reg_val &= ~(1<<(uint8_t)clk);
    }
    else
    {
        reg_val |= (1<<(uint8_t)clk);
    }

    si5351_write(SI5351_OUTPUT_ENABLE_CTRL, reg_val);
}

/*******************************/
/* Suggested private functions */
/*******************************/

/*
 * Calculate best rational approximation for a given fraction
 * taking into account restricted register size, e.g. to find
 * appropriate values for a pll with 5 bit denominator and
 * 8 bit numerator register fields, trying to set up with a
 * frequency ratio of 3.1415, one would say:
 *
 * rational_best_approximation(31415, 10000,
 *              (1 << 8) - 1, (1 << 5) - 1, &n, &d);
 *
 * you may look at given_numerator as a fixed point number,
 * with the fractional part size described in given_denominator.
 *
 * for theoretical background, see:
 * http://en.wikipedia.org/wiki/Continued_fraction
 */

static void rational_best_approximation(
    uint64_t given_numerator, uint64_t given_denominator,
    uint32_t max_numerator, uint32_t max_denominator,
    uint32_t *best_numerator, uint32_t *best_denominator)
{
    uint64_t n, d, n0, d0, n1, d1;
    n = given_numerator;
    d = given_denominator;
    n0 = d1 = 0;
    n1 = d0 = 1;
    for (;;)
    {
        uint64_t t, a;
        if ((n1 > max_numerator) || (d1 > max_denominator))
        {
            n1 = n0;
            d1 = d0;
            break;
        }
        if (d == 0)
            break;
        t = d;
        a = n / d;
        d = n % d;
        n = t;
        t = n0 + a * n1;
        n0 = n1;
        n1 = t;
        t = d0 + a * d1;
        d0 = d1;
        d1 = t;
    }
    *best_numerator = (uint32_t)n1;
    *best_denominator = (uint32_t)d1;
}

static void si5351_set_pll(uint32_t a, uint32_t b, uint32_t c, enum si5351_pll pll)
{//Set PLL parameters
    uint8_t params[8];
    uint8_t i = 0;
    uint8_t temp;
    uint32_t p1, p2, p3;

    p3  = c;
    p2  = (128 * b) % c;
    p1  = 128 * a;
    p1 += (128 * b / c);
    p1 -= 512;

    /* Registers 26-27 */
    temp = ((p3 >> 8) & 0xFF);
    params[i++] = temp;

    temp = (uint8_t)(p3  & 0xFF);
    params[i++] = temp;

    /* Register 28 */
    temp = (uint8_t)((p1 >> 16) & 0x03);
    params[i++] = temp;

    /* Registers 29-30 */
    temp = (uint8_t)((p1 >> 8) & 0xFF);
    params[i++] = temp;

    temp = (uint8_t)(p1  & 0xFF);
    params[i++] = temp;

    /* Register 31 */
    temp = (uint8_t)((p3 >> 12) & 0xF0);
    temp += (uint8_t)((p2 >> 16) & 0x0F);
    params[i++] = temp;

    /* Registers 32-33 */
    temp = (uint8_t)((p2 >> 8) & 0xFF);
    params[i++] = temp;

    temp = (uint8_t)(p2  & 0xFF);
    params[i++] = temp;

    if (pll == SI5351_PLLA)
    {
        si5351_write_bulk(SI5351_PLLA_PARAMETERS, 8, params);
        //Any change to the feedback Multisynth require a PLL reset
        si5351_write(SI5351_PLL_RESET, SI5351_PLL_RESET_A);
    }
    else
    {
        si5351_write_bulk(SI5351_PLLB_PARAMETERS, 8, params);
        //Any change to the feedback Multisynth require a PLL reset
        si5351_write(SI5351_PLL_RESET, SI5351_PLL_RESET_B);
    }
}

static void si5351_set_ms(uint32_t a, uint32_t b, uint32_t c, uint8_t rdiv, enum si5351_clock clk)
{
    uint8_t params[8];
    uint8_t i = 0;
    uint8_t temp;
    uint32_t p1, p2, p3;

    if (a == 4)
    {
        p1 = 0;
        p2 = 0;
        p3 = 1;
    }
    else
    {
        p3  = c;
        p2  = (128 * b) % c;
        p1  = 128 * a;
        p1 += (128 * b / c);
        p1 -= 512;
    }

    /* Registers 42-43 */
    temp = (uint8_t)((p3 >> 8) & 0xFF);
    params[i++] = temp;

    temp = (uint8_t)(p3  & 0xFF);
    params[i++] = temp;

    /* Register 44 (or 52)*/
    temp = (uint8_t)((p1 >> 16) & 0x03);
    if (a == 4) //Set div by 4 bits
        temp |= 0x0C;
    temp |= ((rdiv & 7) << 4);
    params[i++] = temp;

    /* Registers 45-46 */
    temp = (uint8_t)((p1 >> 8) & 0xFF);
    params[i++] = temp;

    temp = (uint8_t)(p1  & 0xFF);
    params[i++] = temp;

    /* Register 47 */
    temp = (uint8_t)((p3 >> 12) & 0xF0);
    temp += (uint8_t)((p2 >> 16) & 0x0F);
    params[i++] = temp;

    /* Registers 48-49 */
    temp = (uint8_t)((p2 >> 8) & 0xFF);
    params[i++] = temp;

    temp = (uint8_t)(p2  & 0xFF);
    params[i++] = temp;

    /* Write the parameters */
    if (clk == SI5351_CLK0)
    {
        si5351_write_bulk(SI5351_CLK0_PARAMETERS, 8, params);
    }
    else if (clk == SI5351_CLK1)
    {
        si5351_write_bulk(SI5351_CLK1_PARAMETERS, 8, params);
    }
}


static void set_multisynth_alt(uint32_t freq, enum si5351_clock clk)
{
    uint32_t a, b, c;
//    uint32_t ap, bp, cp;
static    uint32_t ap, bp, cp;

static    uint32_t ap_=0, bp_=0, cp_=0;

static uint64_t nom, den;
    double divpll, divms, fpll, k;
    uint8_t rdiv = SI5351_OUTPUT_CLK_DIV_1;

    if (freq < SI5351_MULTISYNTH_MIN_FREQ / 128)
        freq = SI5351_MULTISYNTH_MIN_FREQ / 128;
    else if (freq > SI5351_MULTISYNTH_MAX_FREQ)
        freq = SI5351_MULTISYNTH_MAX_FREQ;

   if (freq >= 112500000ul)
    {//Use output multisynth constant divider = 4, calculate PLL feedback multisynth to set desired frequency
        if (freq >= SI5351_MULTISYNTH_DIVBY4_FREQ)
            divms = 4.0;
        else
            divms = 6.0;
        divpll = (freq / ((double)si5351_XTAL_FREQ)) * divms;
        //Calculate ap, bp, cp (pll feedback multisynth parameters)
        ap = (uint32_t)floor(divpll);
        bp = 0;
        cp = 1;
        k = divpll - ap;
        nom = (uint64_t)(k * 0x6FFFFFFFFFFFFFFFull);
        den = 0x6FFFFFFFFFFFFFFFull;
        rational_best_approximation(nom, den, SI5351_PLL_B_MAX, SI5351_PLL_C_MAX, &bp, &cp);
        fpll = si5351_XTAL_FREQ * ((double)ap + (double)bp / (double)cp);
    }
    else
    {//Set PLL to maximum frequency, calculate output multisynth divider
        uint32_t freq_m = freq;

        //Handle frequencies below 1 MHz
        if (freq < (SI5351_MULTISYNTH_MIN_FREQ / 64))
            rdiv = SI5351_OUTPUT_CLK_DIV_128;
        else if (freq < (SI5351_MULTISYNTH_MIN_FREQ / 32))
            rdiv = SI5351_OUTPUT_CLK_DIV_64;
        else if (freq < (SI5351_MULTISYNTH_MIN_FREQ / 16))
            rdiv = SI5351_OUTPUT_CLK_DIV_32;
        else if (freq < (SI5351_MULTISYNTH_MIN_FREQ / 8))
            rdiv = SI5351_OUTPUT_CLK_DIV_16;
        else if (freq < (SI5351_MULTISYNTH_MIN_FREQ / 4))
            rdiv = SI5351_OUTPUT_CLK_DIV_8;
        else if (freq < (SI5351_MULTISYNTH_MIN_FREQ / 2))
            rdiv = SI5351_OUTPUT_CLK_DIV_4;
        else if (freq < (SI5351_MULTISYNTH_MIN_FREQ))
            rdiv = SI5351_OUTPUT_CLK_DIV_2;
        freq_m *= (1 << rdiv);

        divpll = ((double)SI5351_PLL_VCO_MAX) / ((double)si5351_XTAL_FREQ);
        //Calculate ap, bp, cp (pll feedback multisynth parameters)
        ap = (uint32_t)floor(divpll);
        bp = 0;
        cp = 1;
        k = divpll - ap;
        nom = (uint64_t)(k * 0x6FFFFFFFFFFFFFFFull);
        den = 0x6FFFFFFFFFFFFFFFull;
        rational_best_approximation(nom, den, SI5351_PLL_B_MAX, SI5351_PLL_C_MAX, &bp, &cp);
        fpll = si5351_XTAL_FREQ * ((double)ap + (double)bp / (double)cp);
        divms = fpll / (double)freq_m;
    }

    //Calculate a, b, c (output multisynth parameters) from divms calculated abvge
    b = 0;
    c = 1;
    if (divms == 4.0)
        a = 4; //MS divider is integer
    else if (divms == 6.0)
        a = 6; //MS divider is integer
    else
    {//MS divider is fractional
        a = (uint32_t)floor(divms);
        k = divms - a;
        nom = (uint64_t)(k * 0x6FFFFFFFFFFFFFFFull);
        den = 0x6FFFFFFFFFFFFFFFull;
        rational_best_approximation(nom, den, SI5351_PLL_B_MAX, SI5351_PLL_C_MAX, &b, &c);
    }

    //Write PLL parameters
    if (clk == SI5351_CLK0)
    {
        //si5351_set_pll(ap, bp, cp, SI5351_PLLA);
      if( (ap_!=ap)||(bp_!=bp)||(cp_!=cp)){
          si5351_set_pll(ap, bp, cp, SI5351_PLLA);
          ap_=ap, bp_=bp, cp_=cp;
        }
        si5351_set_ms(a, b, c, rdiv, clk);
        si5351_set_clk_control(clk, SI5351_PLLA, (a == 4) || (a == 6), SI5351_DRIVE_8MA);
    }
    else if (clk == SI5351_CLK1)
    {
        si5351_set_pll(ap, bp, cp, SI5351_PLLB);
        si5351_set_ms(a, b, c, rdiv, clk);
        si5351_set_clk_control(clk, SI5351_PLLB, (a == 4) || (a == 6), SI5351_DRIVE_8MA);
    }
    
#if DEBUG
    {
        //Calculate and print actual frequency
        double fpll_actual;
        double f_actual;
        double divms;
        double diff;
        fpll_actual = si5351_XTAL_FREQ * ((double)ap + (double)bp / (double)cp);
        divms = ((double)a + (double)b / (double)c);
        f_actual = (fpll_actual / divms) / (1 << rdiv);
        diff = f_actual - freq;
        //if (abs(diff) > 0.5)
        //    DBGPRINT("CLK%d actual freq: %.1f Hz, desired %u, diff %.1f\n", clk, f_actual, (unsigned int)freq, f_actual - freq);
        //sprintf(text, "diff %.1f", diff);
        //TM_ILI9341_Puts(175, 0, text , &TM_Font_7x10, RED, BLACK);
        sprintf(text, "%.2f", freq/1000.0);
        TM_ILI9341_Puts(180, 0, text , &TM_Font_7x10, RED, BLACK);
        sprintf(text, "%.2f", f_actual/1000.0);
        TM_ILI9341_Puts(170, 0, text , &TM_Font_7x10, RED, BLACK);
    }
 #endif
}

static uint8_t si5351_write_bulk(uint8_t addr, uint8_t bytes, uint8_t *data)
{
    int i;

    i2c_start();
    i2c_write(SI5351_BUS_BASE_ADDR);
    i2c_write(addr);
    for(i = 0; i < bytes; i++)
    {
        i2c_write(data[i]);
    }
    i2c_stop();
    return 0;
}

static uint8_t si5351_write(uint8_t addr, uint8_t data)
{
    i2c_start();
    i2c_write(SI5351_BUS_BASE_ADDR);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
    return 0;
}

static uint8_t si5351_read(uint8_t addr, uint8_t *data)
{
    i2c_start();
    i2c_write(SI5351_BUS_BASE_ADDR);
    i2c_write(addr);
    i2c_stop();
    i2c_start();
    i2c_write(SI5351_BUS_BASE_ADDR | 1);

    *data = i2c_read_nack(1);
    i2c_stop();
    return 0;
}

static void si5351_set_clk_control(enum si5351_clock clk, enum si5351_pll pll, int isIntegerMode, enum si5351_drive drive)
{
    //Bit  D7       D6      D5      D4       D3 D2         D1 D0
    //Name CLK0_PDN MS0_INT MS0_SRC CLK0_INV CLK0_SRC[1:0] CLK0_IDRV[1:0]
    uint8_t reg_val = 0x0C; //Select this multisynth as the source for clk output,

    reg_val |= ((uint8_t)drive) & 3;//Set drive strength

    if(pll == SI5351_PLLB)
        reg_val |= SI5351_CLK_PLL_SELECT; //Select PLLB as the source for this multisynth, otherwise PLLA will be used

    if (isIntegerMode) //Set integer mode for this multisynth
        reg_val |= SI5351_CLK_INTEGER_MODE;

    //clk value can be used as offset to address proper register
    si5351_write(SI5351_CLK0_CTRL + (uint8_t)clk, reg_val);
}

uint8_t si5351_read_device_status(void)
{
    uint8_t status = 0;
    si5351_read(0, &status);
    return status;

}
//------------------------------------------------------------------------------






//------------------------------------------------------------------------------
//-------------------------------------------------------

void i2c_init(void){

 /* GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, DISABLE);

  GPIO_WriteBit(I2C_PORT, SDA|SCL, Bit_SET);
  // Configure I2C2 pins: SCL and SDA 
  GPIO_InitStructure.GPIO_Pin =  SDA | SCL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(I2C_PORT, &GPIO_InitStructure);*/
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD  ,ENABLE);
  
 
  GPIO_InitStructure.GPIO_Pin = SDA | SCL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(I2C_PORT, &GPIO_InitStructure);
  GPIO_SetBits( I2C_PORT, SDA | SCL);

}
//-------------------------------------------------------

void i2c_start( void ){
  GPIO_InitTypeDef  GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin =  SDA ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(I2C_PORT, &GPIO_InitStructure);

  GPIO_WriteBit(I2C_PORT, SDA, Bit_SET);
  for( int i = 0; i<pI2C_Speed; ++i ) asm("nop");
  GPIO_WriteBit(I2C_PORT, SCL, Bit_SET);
  for( int i = 0; i<pI2C_Speed; ++i ) asm("nop");
  GPIO_Init(I2C_PORT, &GPIO_InitStructure);
  GPIO_WriteBit(I2C_PORT, SDA, Bit_RESET);
  for( int i = 0; i<pI2C_Speed; ++i ) asm("nop");
  GPIO_WriteBit(I2C_PORT, SCL, Bit_RESET);
  for( int i = 0; i<pI2C_Speed; ++i )asm("nop");
  for( int i = 0; i<pI2C_Speed; ++i )asm("nop");
  for( int i = 0; i<pI2C_Speed; ++i )asm("nop");
  for( int i = 0; i<pI2C_Speed; ++i )asm("nop");

}
//-------------------------------------------------------

void i2c_stop( void ){

  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin =  SDA ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(I2C_PORT, &GPIO_InitStructure);

  GPIO_WriteBit(I2C_PORT, SDA, Bit_RESET);
  for( int i = 0; i<pI2C_Speed; ++i ) asm("nop");

  GPIO_WriteBit(I2C_PORT, SCL, Bit_SET);
  for( int i = 0; i<pI2C_Speed; ++i ) asm("nop");
  GPIO_WriteBit(I2C_PORT, SDA, Bit_SET);
  for( int i = 0; i<pI2C_Speed; ++i ) asm("nop");
  for( int i = 0; i<pI2C_Speed; ++i ) asm("nop");
  for( int i = 0; i<pI2C_Speed; ++i )asm("nop");
  for( int i = 0; i<pI2C_Speed; ++i )asm("nop");
}
//-------------------------------------------------------
int i2c_write( unsigned char byte ){
  unsigned char temp=byte;

  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin =  SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(I2C_PORT, &GPIO_InitStructure);

  for( int j = 0; j<8; ++j ){

    for( int i = 0; i<pI2C_Speed; ++i ) asm("nop");

    if( (temp&0x80) == 0x80 )
      GPIO_WriteBit(I2C_PORT, SDA, Bit_SET);
    else
      GPIO_WriteBit(I2C_PORT, SDA, Bit_RESET);
    temp <<=1;

    for( int i = 0; i<pI2C_Speed; ++i )asm("nop");
    GPIO_WriteBit(I2C_PORT, SCL, Bit_SET);
    for( int i = 0; i<pI2C_Speed; ++i )asm("nop");
    GPIO_WriteBit(I2C_PORT, SCL, Bit_RESET);
  }
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(I2C_PORT, &GPIO_InitStructure);

  for( int i = 0; i<pI2C_Speed; ++i )asm("nop");
  GPIO_WriteBit(I2C_PORT, SCL, Bit_SET);
  for( int i = 0; i<pI2C_Speed; ++i )asm("nop");
  //read ack
  GPIO_WriteBit(I2C_PORT, SCL, Bit_RESET);



  return 0;
}
//-------------------------------------------------------

unsigned char i2c_read_nack( int need_ack ){
  unsigned char temp=0;

  GPIO_InitTypeDef  GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin =  SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(I2C_PORT, &GPIO_InitStructure);

  for( int j = 0; j<8; ++j ){
    for( int i = 0; i<pI2C_Speed; ++i ) asm("nop");
    GPIO_WriteBit(I2C_PORT, SCL, Bit_SET);
    //read bit
    if (GPIO_ReadInputDataBit(I2C_PORT,SDA) == Bit_SET)
      temp |=(1<<(7-j)) ;

    for( int i = 0; i<pI2C_Speed; ++i ) asm("nop");
    GPIO_WriteBit(I2C_PORT, SCL, Bit_RESET);
  }

  if( need_ack ){
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(I2C_PORT, &GPIO_InitStructure);
    GPIO_WriteBit(I2C_PORT, SDA, Bit_RESET);
  }
  for( int i = 0; i<pI2C_Speed; ++i )asm("nop");
  GPIO_WriteBit(I2C_PORT, SCL, Bit_SET);
  for( int i = 0; i<pI2C_Speed; ++i ) asm("nop");
  GPIO_WriteBit(I2C_PORT, SCL, Bit_RESET);



  return temp;
}