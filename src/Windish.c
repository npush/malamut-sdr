//#include "stm32f4xx.h"
#include "main.h"
#include "Windish.h"

arm_cfft_instance_f32* CFFT_imbalance_init_struct;
COMPLEX_float IQ_imbalance_input[FFT_IQ_imbalanse_size/2];
COMPLEX_float Alpha_Beta_cmplx[FFT_IQ_imbalanse_size/2];//.re- Alpha,.im-Beta
COMPLEX_float IQ_imbalance_cfft_input[FFT_IQ_imbalanse_size];
COMPLEX_float buf_m_fK1K2[FFT_IQ_imbalanse_size/2];
float buf_m_fPWR[FFT_IQ_imbalanse_size/2];
COMPLEX_float m_cpxK2[FFT_IQ_imbalanse_size/2];
COMPLEX_float m_cpxK1[FFT_IQ_imbalanse_size/2];
int m_iFrameNum;
int m_bCarrDet[FFT_IQ_imbalanse_size/2];
int m_iCarr_Pres_Cnt[FFT_IQ_imbalanse_size/2];//Счетчик присутствия сигнала на частоте
int m_iCarr_Reset_Cnt[FFT_IQ_imbalanse_size/2];//Счетчик отсутствия сигнала на частоте
float m_fMinPwr=0;
float m_fAvgPwr[FFT_IQ_imbalanse_size];
int m_iNumCarriers;
int I_START=0;

//extern COMPLEX_float cfft_buffer[FFT_FILTER_SIZE];//внутренни буффер для БПФ

 float HP_FILTER[256] CCM_RAM=
{
1.81856861731388596e-17,  1.43066333779806598e-08,  -5.83775303653055744e-08,  2.84220234554252715e-07,
-1.00771189066872768e-08,  -2.23188615922076394e-08,  -2.52739808748181360e-07,  3.30788591962338785e-07,
-2.28820518505257198e-08,  1.12652219774744822e-08,  -3.93976476054103131e-07,  4.31785744464674437e-07,
-3.22326662027805355e-08,  -2.55326446096937332e-08,  -6.31971558862076677e-07,  5.84023745461847987e-07,
-5.25979157450784385e-08,  2.83366663467177204e-10,  -8.52178585641905073e-07,  8.14423204598124784e-07,
-6.14375840757297453e-08,  -3.08965748541112362e-08,  -1.17890877054938059e-06,  1.10446559192789451e-06,
-7.45580804317746981e-08,  2.90622520493499792e-09,  -1.47921611154400168e-06,  1.45518981670145209e-06,
-3.96151722537041451e-08,  7.52080575594504808e-09,  -1.76695138848215194e-06,  1.71748838878604815e-06,
1.12997704998507587e-07,  1.87120985706279441e-07,  -1.50170458357301584e-06,  1.45248739066849986e-06,
6.99435658668701166e-07,  6.38415712406192881e-07,  4.34243647928767018e-07,  -7.92389325598996663e-07,
1.35020455702128395e-06,  1.13247930749738046e-06,  1.92105625945737657e-06,  -2.51970603762363687e-06,
-6.56451098198229666e-05,  -5.74508957120745123e-05,  -8.64622578564899769e-04,  9.81206654752875274e-04,
5.94314686547703364e-03,  5.27492125864598149e-03,  2.00074047866246607e-02,  -2.23590925930068352e-02,
-6.12774413363852410e-02,  -5.53414415779216462e-02,  -1.20915495484505697e-01,  1.32437024983128787e-01,
2.36668648489700317e-01,  2.18956581828214980e-01,  3.39801537053710079e-01,  -3.61288438132257139e-01,
-4.83467989864296954e-01,  -4.64257243436597466e-01,  -5.72295346334941390e-01,  5.80589690131863234e-01,
6.40571838496870938e-01,  6.52196507294254268e-01,  7.03913523540751762e-01,  -6.65133276657339567e-01,
-6.64995999271572824e-01,  -7.35438181028638338e-01,  -7.56269464833683736e-01,  6.52062686045915996e-01,
6.34314456568917606e-01,  7.72943301961550255e-01,  7.88344599205485608e-01,  -6.15229799083339746e-01,
-5.95697847770491506e-01,  -8.03206221762613803e-01,  -8.17585191097562336e-01,  5.75808291758965751e-01,
5.55570920251694300e-01,  8.31470186002201128e-01,  8.44855143013565457e-01,  -5.34999814583799749e-01,
-5.14102728972504974e-01,  -8.57728661821534066e-01,  -8.70085231335247622e-01,  4.92895731526761138e-01,
4.71396600019217016e-01,  8.81921114229839320e-01,  8.93225849837506081e-01,  -4.49613525574446393e-01,
-4.27555258104101443e-01,  -9.03989480793760958e-01,  -9.14208559052880787e-01,  4.05239493543594786e-01,
3.82683307892433322e-01,  9.23879409867281454e-01,  9.32993752027480250e-01,  -3.59896557933229777e-01,
-3.36889967391716116e-01,  -9.41544195322604249e-01,  -9.49527494743956035e-01,  3.13680488364192767e-01,
2.90284608119973919e-01,  9.56940275097705850e-01,  9.63776618661622897e-01,  -2.66713839240435957e-01,
-2.42980247279871608e-01,  -9.70031339261072878e-01,  -9.75701763187527882e-01,  2.19100316430546838e-01,
1.95090287739118240e-01,  9.80785255922378196e-01,  9.85277944511769421e-01,  -1.70962727010602167e-01,
-1.46730509202006698e-01,  -9.89176571194776311e-01,  -9.92479371652644104e-01,  1.22409917758777506e-01,
9.80171255341558134e-02,  9.95184718838718951e-01,  9.97290586316168737e-01,  -7.35652844984727716e-02,
-4.90676845904052319e-02,  -9.98795505737348432e-01,  -9.99698806413063412e-01,  2.45405316429064616e-02,
-1.16932085299405932e-17,  9.99999996893503607e-01,  9.99698806413060526e-01,  2.45405316429166617e-02,
4.90676845904152031e-02,  -9.98795505737344991e-01,  -9.97290586316164962e-01,  -7.35652844984828747e-02,
-9.80171255341659858e-02,  9.95184718838714399e-01,  9.92479371652638775e-01,  1.22409917758788261e-01,
1.46730509202017800e-01,  -9.89176571194770426e-01,  -9.85277944511762538e-01,  -1.70962727010613769e-01,
-1.95090287739129953e-01,  9.80785255922370869e-01,  9.75701763187520221e-01,  2.19100316430558634e-01,
2.42980247279883071e-01,  -9.70031339261064662e-01,  -9.63776618661614348e-01,  -2.66713839240447392e-01,
-2.90284608119984966e-01,  9.56940275097696968e-01,  9.49527494743946710e-01,  3.13680488364204146e-01,
3.36889967391727330e-01,  -9.41544195322594368e-01,  -9.32993752027469814e-01,  -3.59896557933240824e-01,
-3.82683307892444313e-01,  9.23879409867270018e-01,  9.14208559052869352e-01,  4.05239493543604778e-01,
4.27555258104110492e-01,  -9.03989480793749522e-01,  -8.93225849837494756e-01,  -4.49613525574454997e-01,
-4.71396600019225009e-01,  8.81921114229828218e-01,  8.70085231335236520e-01,  4.92895731526768799e-01,
5.14102728972512191e-01,  -8.57728661821522853e-01,  -8.44855143013553911e-01,  -5.34999814583806743e-01,
-5.55570920251700850e-01,  8.31470186002189582e-01,  8.17585191097550790e-01,  5.75808291758971857e-01,
5.95697847770496614e-01,  -8.03206221762602590e-01,  -7.88344599205474283e-01,  -6.15229799083344631e-01,
-6.34314456568921825e-01,  7.72943301961538709e-01,  7.56269464833671523e-01,  6.52062686045919548e-01,
6.64995999271575822e-01,  -7.35438181028625460e-01,  -7.03913523540738106e-01,  -6.65133276657341899e-01,
-6.40571838496870938e-01,  6.52196507294246719e-01,  5.72295346334928401e-01,  5.80589690131862679e-01,
4.83467989864295289e-01,  -4.64257243436585698e-01,  -3.39801537053700087e-01,  -3.61288438132254308e-01,
-2.36668648489696126e-01,  2.18956581828206209e-01,  1.20915495484497759e-01,  1.32437024983123180e-01,
6.12774413363786352e-02,  -5.53414415779143187e-02,  -2.00074047866177773e-02,  -2.23590925929990636e-02,
-5.94314686546870696e-03,  5.27492125863937567e-03,  8.64622578558571497e-04,  9.81206654744382067e-04,
6.56451098111077158e-05,  -5.74508957060237968e-05,  -1.92105625368421684e-06,  -2.51970602879736383e-06,
-1.35020454794521072e-06,  1.13247930194626534e-06,  -4.34243653424370990e-07,  -7.92389335146914675e-07,
-6.99435668410908207e-07,  6.38415717790774551e-07,  1.50170458862453060e-06,  1.45248740088255168e-06,
-1.12997694035055218e-07,  1.87120980932320435e-07,  1.76695138442983790e-06,  1.71748837823892941e-06,
3.96151617620965624e-08,  7.52080925314757565e-09,  1.47921611470813730e-06,  1.45518982691550391e-06,
7.45580903682707685e-08,  2.90622220733283143e-09,  1.17890876782933418e-06,  1.10446558188037613e-06,
6.14375741531114628e-08,  -3.08965723561094308e-08,  8.52178587917862274e-07,  8.14423214562376430e-07,
5.25979256399411454e-08,  2.83364609554581648e-10,  6.31971556919186384e-07,  5.84023735664129795e-07,
3.22326565715957969e-08,  -2.55326429998703475e-08,  3.93976477608415365e-07,  4.31785754220759266e-07,
2.28820619882497134e-08,  1.12652204231622477e-08,  2.52739807138357975e-07,  3.30788581172358764e-07,
1.00771074887373580e-08,  -2.23188597603396488e-08,  5.83775326412627749e-08,  2.84220246575886404e-07,
};


//--------------------------------------------------------------------------------------------------

/*Вычисление мощности спектра, средней мощности, шумовой дорожки */
void GetSpecPwr (COMPLEX_float* input_spec)
{
  int i;
  float tmp= 3.0e38f, tmp_Pwr=0;
  float beta=0.99f;
  float alpha=(1-beta);
  for (i = 0; i < FFT_IQ_imbalanse_size; i+=4)
  {
    tmp_Pwr =(input_spec[i].re*input_spec[i].re) + (input_spec[i].im*input_spec[i].im);
    m_fAvgPwr[i]=(m_fAvgPwr[i]*beta)+(tmp_Pwr*alpha);

    tmp_Pwr =(input_spec[i+1].re*input_spec[i+1].re) + (input_spec[i+1].im*input_spec[i+1].im);
    m_fAvgPwr[i+1]=(m_fAvgPwr[i+1]*beta)+(tmp_Pwr*alpha);

    tmp_Pwr =(input_spec[i+2].re*input_spec[i+2].re) + (input_spec[i+2].im*input_spec[i+2].im);
    m_fAvgPwr[i+2]=(m_fAvgPwr[i+2]*beta)+(tmp_Pwr*alpha);

    tmp_Pwr =(input_spec[i+3].re*input_spec[i+3].re) + (input_spec[i+3].im*input_spec[i+3].im);
    m_fAvgPwr[i+3]=(m_fAvgPwr[i+3]*beta)+(tmp_Pwr*alpha);

    if (m_fAvgPwr[i] <tmp)
      tmp = m_fAvgPwr[i];

    if (m_fAvgPwr[i+1] <tmp)
      tmp = m_fAvgPwr[i+1];

    if (m_fAvgPwr[i+2] <tmp)
      tmp = m_fAvgPwr[i+2];

    if (m_fAvgPwr[i+3] <tmp)
      tmp = m_fAvgPwr[i+3];

  }

  m_fMinPwr=(m_fMinPwr*beta)+(alpha*tmp);
  // Маскирую бины около нуля
  m_fAvgPwr[0] = m_fAvgPwr[Mask0Hz];
	
}
//--------------------------------------------------------------------------------------------------

float Q1[FFT_IQ_imbalanse_size/2],Q2[FFT_IQ_imbalanse_size/2],
Q3[FFT_IQ_imbalanse_size/2],Q4[FFT_IQ_imbalanse_size/2],Q5[FFT_IQ_imbalanse_size/2];
//--------------------------------------------------------------------------------------------------
void Chebyshev_CMPLX ( COMPLEX_float* input)
{

  /*Эта функция была написана, чтобы сократить общее количество вычислений при интерполяции
  Alpha и Beta за счет скоращения   одних и тех же проходов циклов*/
  int i;

  COMPLEX_float a0,a1,a2,a3, a4;
  float Mx=0, Q0=1,H0=0,tmp=0,H1=0, beta2=0, beta3=0, beta4=0,tmp_coef=0, H2=0,H3=0,H4=0;
  H0=m_iNumCarriers;
  a0.re=a0.im=a1.re=a1.im=a2.re=a2.im=a3.re=a3.im=a4.re=a4.im=0;
  for (i = I_START; i < FFT_IQ_imbalanse_size/2; i++)
  {	
    if (m_bCarrDet[i]==1)
    {
      Mx+=i;
      a0.re+=input[i].re;
      a0.im+=input[i].im;
    }
  }

  Mx=Mx/H0;
  a0.re=a0.re/H0;
  a0.im=a0.im/H0;
  if (H0<=1)
    goto M;//Если точек меньше трех, то выхожу на расчет выхода

  for (i = I_START; i < FFT_IQ_imbalanse_size/2; i++)
  {
    Q1[i]=0;
    if (m_bCarrDet[i]==1)
    {
      Q1[i]=i-Mx;
      tmp=Q1[i]*Q1[i];
      H1+=tmp;
      a1.re+=Q1[i]*input[i].re;
      a1.im+=Q1[i]*input[i].im;
      beta2+=i*tmp;
    }
  }

  a1.re=a1.re/H1;
  a1.im=a1.im/H1;
  if (H0<=2)
    goto M;//Если точек меньше трех, то выхожу на расчет выхода
  //Иначе интерполирую параболой

  beta2=beta2/H1;
  tmp_coef=(H1/H0);

  for (i = I_START; i < FFT_IQ_imbalanse_size/2; i++)
  {
    Q2[i]=0;
    if (m_bCarrDet[i]==1)
          {
          Q2[i]=Q1[i]*(i-beta2)-tmp_coef;
          tmp=Q2[i]*Q2[i];
          H2+=tmp;
          a2.re+=Q2[i]*input[i].re;
          a2.im+=Q2[i]*input[i].im;
          beta3+=i*tmp;
          }
  }
  a2.re=a2.re/H2;
  a2.im=a2.im/H2;
  if (H0<=3)
    goto M;


  beta3=beta3/H2;
  tmp_coef=(H2/H1);
  for (i = I_START; i < FFT_IQ_imbalanse_size/2; i++)
  {
    Q3[i]=0;
          if (m_bCarrDet[i]==1)
          {
          Q3[i]=Q2[i]*(i-beta3)-Q1[i]*tmp_coef;
          tmp=Q3[i]*Q3[i];
          H3+=tmp;
          a3.re+=Q3[i]*input[i].re;
          a3.im+=Q3[i]*input[i].im;
          beta4+=i*tmp;
          }
  }
  a3.re=a3.re/H3;
  a3.im=a3.im/H3;
  if (H0<=4)
    goto M;


  beta4=beta4/H3;
  tmp_coef=(H3/H2);
  for (i = I_START; i < FFT_IQ_imbalanse_size/2; i++)
  {
    Q4[i]=0;
          if (m_bCarrDet[i]==1)
          {
          Q4[i]=Q3[i]*(i-beta4)-Q2[i]*tmp_coef;
          H4+=Q4[i]*Q4[i];
          a4.re+=Q4[i]*input[i].re;
          a4.im+=Q4[i]*input[i].im;
          }
  }
  a4.re=a4.re/H4;
  a4.im=a4.im/H4;

M:
    for (i = 0; i < FFT_IQ_imbalanse_size/2; i+=4)
  {
    input[i].re=(a0.re*Q0)+(a1.re*Q1[i])+ (a2.re*Q2[i])+(a3.re*Q3[i])+(a4.re*Q4[i]);
    input[i].im=(a0.im*Q0)+(a1.im*Q1[i])+ (a2.im*Q2[i])+(a3.im*Q3[i])+(a4.im*Q4[i]);

    input[i+1].re=(a0.re*Q0)+(a1.re*Q1[i+1])+ (a2.re*Q2[i+1])+(a3.re*Q3[i+1])+(a4.re*Q4[i+1]);
    input[i+1].im=(a0.im*Q0)+(a1.im*Q1[i+1])+ (a2.im*Q2[i+1])+(a3.im*Q3[i+1])+(a4.im*Q4[i+1]);

    input[i+2].re=(a0.re*Q0)+(a1.re*Q1[i+2])+ (a2.re*Q2[i+2])+(a3.re*Q3[i+2])+(a4.re*Q4[i+2]);
    input[i+2].im=(a0.im*Q0)+(a1.im*Q1[i+2])+ (a2.im*Q2[i+2])+(a3.im*Q3[i+2])+(a4.im*Q4[i+2]);

    input[i+3].re=(a0.re*Q0)+(a1.re*Q1[i+3])+ (a2.re*Q2[i+3])+(a3.re*Q3[i+3])+(a4.re*Q4[i+3]);
    input[i+3].im=(a0.im*Q0)+(a1.im*Q1[i+3])+ (a2.im*Q2[i+3])+(a3.im*Q3[i+3])+(a4.im*Q4[i+3]);
  }
}
//--------------------------------------------------------------------------------------------------
void InitParams(int n_carriers)
{
  int i;
  float tmp;
  if (n_carriers>0)
  {
    Chebyshev_CMPLX (( COMPLEX_float *)&Alpha_Beta_cmplx);
    for (i = 1; i < FFT_IQ_imbalanse_size/2; i++)
    {
      tmp=(2*Alpha_Beta_cmplx[i].re);
      m_cpxK2[i].re = (1 - Alpha_Beta_cmplx[i].re)/tmp;
      m_cpxK2[i].im =m_cpxK1[i].im= -(Alpha_Beta_cmplx[i].im)/tmp;
      m_cpxK1[i].re = (1 + Alpha_Beta_cmplx[i].re)/tmp;
    }
  }
}
//--------------------------------------------------------------------------------------------------

void UpdateIQParams( COMPLEX_float* input_spec)
{
int i=0, iBinNum=0;
 COMPLEX_float m_fK1K2;
float tmp_m_fMinPwr=0;
i=I_START;//В зависимости от моды смотрю начиная с разных бинов:
// Для АМ и ФМ с первого бина, т.к. там все попадает в зеркальный канал
//Для SSB только выше четверти частоты дискретизации, т.к. все что ниже на зеркалку не влияет.


  if(m_fMinPwr==0.0f)
    tmp_m_fMinPwr=1;//чтоб избежать деления на ноль
  else
    tmp_m_fMinPwr=m_fMinPwr;

	while (i < (FFT_IQ_imbalanse_size/2))
	{
if (((m_fAvgPwr[FFT_IQ_imbalanse_size-i]/tmp_m_fMinPwr) <= THRESHOLD)
        &&((m_fAvgPwr[i]/tmp_m_fMinPwr) <= THRESHOLD)) //Если меньше порога, то
        {
          }
	else
	{
       	m_iCarr_Pres_Cnt[i]++;
        buf_m_fPWR[i]+=(input_spec[i].re+input_spec[FFT_IQ_imbalanse_size - i].re)
                      *(input_spec[i].re+input_spec[FFT_IQ_imbalanse_size - i].re)
                  +(input_spec[i].im - input_spec[FFT_IQ_imbalanse_size-i].im)
                    *(input_spec[i].im-input_spec[FFT_IQ_imbalanse_size- i].im) ;
	buf_m_fK1K2[i].re += (input_spec[i].re*input_spec[FFT_IQ_imbalanse_size - i].re)
                  - (input_spec[i].im * input_spec[FFT_IQ_imbalanse_size- i].im);
		
	buf_m_fK1K2[i].im += (input_spec[i].re*input_spec[FFT_IQ_imbalanse_size- i].im)
        +(input_spec[i].im * input_spec[FFT_IQ_imbalanse_size - i].re);
        }
		++i;
	}
	if (m_iFrameNum == (NUM_FRAMES_FOR_UPDATE))
	{
          m_iNumCarriers=0;
           for (iBinNum=I_START;iBinNum<FFT_IQ_imbalanse_size/2;iBinNum++)
           {//если не было сигнала, то обнуляю счетчики
           if ((m_iCarr_Pres_Cnt[iBinNum]) <(NUM_FRAMES_FOR_COLLECTION))
           {
           m_iCarr_Pres_Cnt[iBinNum]=0;
           m_bCarrDet[iBinNum]=0;
           m_iCarr_Reset_Cnt[iBinNum]++;
           if (m_iCarr_Reset_Cnt[iBinNum]>NUM_BLOCKS_FOR_RESET)
           {
            m_iCarr_Reset_Cnt[iBinNum]=0;
            buf_m_fK1K2[iBinNum].re =buf_m_fK1K2[iBinNum].im =buf_m_fPWR[iBinNum]=0;
           }
           }
           else
           {
   m_fK1K2.re=(buf_m_fK1K2[iBinNum].re) / ((buf_m_fPWR[iBinNum]));
   m_fK1K2.im=(buf_m_fK1K2[iBinNum].im)/ (buf_m_fPWR[iBinNum]);
   Alpha_Beta_cmplx[iBinNum].im = -2*m_fK1K2.im;
   Alpha_Beta_cmplx[iBinNum].re=(1 - (Alpha_Beta_cmplx[iBinNum].im * Alpha_Beta_cmplx[iBinNum].im) - (4*m_fK1K2.re));
   Alpha_Beta_cmplx[iBinNum].re = sqrtf(Alpha_Beta_cmplx[iBinNum].re);
            m_iCarr_Reset_Cnt[iBinNum]=0;
            m_bCarrDet[iBinNum]=1;
            m_iNumCarriers++;  //Суммирую количество несущих
            m_iCarr_Pres_Cnt[iBinNum]=0;  //обнуляю счетчики
           }
          }
        InitParams(m_iNumCarriers);
       	m_iFrameNum = 0;
	}
	++m_iFrameNum;
}
//--------------------------------------------------------------------------------------------------
COMPLEX_float Z_m[FFT_IQ_imbalanse_size/2],
Tmp_buf1[FFT_IQ_imbalanse_size/2],
Tmp_buf2[FFT_IQ_imbalanse_size/2], Tmp_K2[FFT_IQ_imbalanse_size/2], Tmp_K1[FFT_IQ_imbalanse_size/2],
Tmp_buf3[FFT_IQ_imbalanse_size/2],Tmp_buf4[FFT_IQ_imbalanse_size/2];

//--------------------------------------------------------------------------------------------------

void CompIQ( COMPLEX_float* input_spec)
{
  int iBinNum;

  for (iBinNum = 1; iBinNum < FFT_IQ_imbalanse_size/2; iBinNum++)
  {
    Z_m[iBinNum].re = input_spec[FFT_IQ_imbalanse_size - iBinNum].re;		// Complex conjugate of Z_m (ie. Z*_m)
    Z_m[iBinNum].im = (-1.0f)*input_spec[FFT_IQ_imbalanse_size- iBinNum].im;
    Tmp_K2[iBinNum].re =(-1.0f)* m_cpxK2[iBinNum].re;
    Tmp_K2[iBinNum].im = m_cpxK2[iBinNum].im;
  }
  arm_cmplx_conj_f32((float*)&m_cpxK1,(float*)&Tmp_K1, (FFT_IQ_imbalanse_size/2));

  arm_cmplx_mult_cmplx_f32((float*)&Tmp_K1,(float*)&input_spec[0],(float*)&Tmp_buf1,(FFT_IQ_imbalanse_size/2));
  arm_cmplx_mult_cmplx_f32((float*)&m_cpxK2,(float*)&Z_m,(float*)&Tmp_buf2,(FFT_IQ_imbalanse_size/2));

  arm_cmplx_mult_cmplx_f32((float*)&Tmp_K2,(float*)&input_spec[0],(float*)&Tmp_buf3,(FFT_IQ_imbalanse_size/2));
  arm_cmplx_mult_cmplx_f32((float*)&m_cpxK1,(float*)&Z_m,(float*)&Tmp_buf4,(FFT_IQ_imbalanse_size/2));

  for (iBinNum = 1; iBinNum < FFT_IQ_imbalanse_size/2; iBinNum++)
  {
    input_spec[iBinNum].re = (Tmp_buf1[iBinNum].re-Tmp_buf2[iBinNum].re);
    input_spec[iBinNum].im = (Tmp_buf1[iBinNum].im-Tmp_buf2[iBinNum].im);

    input_spec[FFT_IQ_imbalanse_size - iBinNum].re = (Tmp_buf4[iBinNum].re+Tmp_buf3[iBinNum].re);
    input_spec[FFT_IQ_imbalanse_size- iBinNum].im = (-1.0f)*(Tmp_buf4[iBinNum].im+Tmp_buf3[iBinNum].im);
  }
}
//--------------------------------------------------------------------------------------------------
void mismatch_init (void)
{
  int i;
  for(i = 0; i < FFT_IQ_imbalanse_size/2; i++)
  {
    m_cpxK2[i].re=m_cpxK2[i].im=0;
    m_cpxK1[i].re=1;
    m_cpxK1[i].im=0;

    Alpha_Beta_cmplx[i].re = 1;
    Alpha_Beta_cmplx[i].im = 0;
    m_bCarrDet[i] = FALSE;
    m_iCarr_Pres_Cnt[i] = 0;
  }
  m_iFrameNum = 0;
  m_iNumCarriers = 0;
  for (i=0;i<FFT_IQ_imbalanse_size;i++)
    m_fAvgPwr[i]=0;

  int tmp=FFT_IQ_imbalanse_size;

  switch (tmp)
  {
  case 2048:
   CFFT_imbalance_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len2048);
  break;
  case 1024:
   CFFT_imbalance_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len1024);
  break;
  case 512:
   CFFT_imbalance_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len512);
  break;
  case 256:
  CFFT_imbalance_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len256);
  break;
  case 128:
   CFFT_imbalance_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len128);
  break;
  case 64:
   CFFT_imbalance_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len64);
  break;
  case 32:
   CFFT_imbalance_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len32);
  break;
  case 16:
   CFFT_imbalance_init_struct =(arm_cfft_instance_f32*)&(arm_cfft_sR_f32_len16);
  break;
  }
}
//--------------------------------------------------------------------------------------------------

void copy_overlap_input_buffer ( COMPLEX_float* dst,COMPLEX_float* input,COMPLEX_float* out )
{
  /*сначала надо перекопировать все из старшей половины буффера в младшую*/
  int i=0;
  for(i=0;i<(FFT_IQ_imbalanse_size/2);i+=4)
  {
    out[i].re= dst[i].re;
    out[i].im= dst[i].im;

    out[i+1].re= dst[i+1].re;
    out[i+1].im= dst[i+1].im;

    out[i+2].re= dst[i+2].re;
    out[i+2].im= dst[i+2].im;

    out[i+3].re= dst[i+3].re;
    out[i+3].im= dst[i+3].im;

  }
  for (i=0;i<(FFT_IQ_imbalanse_size/2);i+=4)
  {
    out[i+(FFT_IQ_imbalanse_size/2)].re= dst[i].re= input[i].re;
    out[i+(FFT_IQ_imbalanse_size/2)].im= dst[i].im= input[i].im;

    out[i+(FFT_IQ_imbalanse_size/2)+1].re= dst[i+1].re= input[i+1].re;
    out[i+(FFT_IQ_imbalanse_size/2)+1].im= dst[i+1].im= input[i+1].im;

    out[i+(FFT_IQ_imbalanse_size/2)+2].re= dst[i+2].re= input[i+2].re;
    out[i+(FFT_IQ_imbalanse_size/2)+2].im= dst[i+2].im= input[i+2].im;

    out[i+(FFT_IQ_imbalanse_size/2)+3].re= dst[i+3].re= input[i+3].re;
    out[i+(FFT_IQ_imbalanse_size/2)+3].im= dst[i+3].im= input[i+3].im;
  }
}

//--------------------------------------------------------------------------------------------------
void copy_overlap_output_cfft_buffer( COMPLEX_float* src,  COMPLEX_float* dst)
{
  int i=0;
  for (i=0;i<(FFT_IQ_imbalanse_size/2);i+=4)
  {
   dst[i].re=src[i+(FFT_IQ_imbalanse_size/2)].re ;
   dst[i].im=src[i+(FFT_IQ_imbalanse_size/2)].im ;
   dst[i+1].re=src[i+1+(FFT_IQ_imbalanse_size/2)].re ;
   dst[i+1].im=src[i+1+(FFT_IQ_imbalanse_size/2)].im ;
   dst[i+2].re=src[i+2+(FFT_IQ_imbalanse_size/2)].re ;
   dst[i+2].im=src[i+2+(FFT_IQ_imbalanse_size/2)].im ;
   dst[i+3].re=src[i+3+(FFT_IQ_imbalanse_size/2)].re ;
   dst[i+3].im=src[i+3+(FFT_IQ_imbalanse_size/2)].im ;
  }
}
//--------------------------------------------------------------------------------------------------

void IQ_imbalance_compensation (int mode)
{
  int i=0;
  switch (mode)
  {
    case 0:
      I_START = I_START_SSB;
      break;
    case 1:
      I_START = I_START_SSB;
      break;
    case 2:
      I_START = I_START_AM_FM;
      break;
    case 3:
      I_START = I_START_AM_FM;
      break;
    default: break;
  }

  for (i=0;i<MISMATCH_COUNT;i++)
  {
    copy_overlap_input_buffer (( COMPLEX_float*)&IQ_imbalance_input,
                               ( COMPLEX_float*)&cfft_buffer[i*FFT_IQ_imbalanse_size/2],
                               ( COMPLEX_float*)&IQ_imbalance_cfft_input);

    arm_cfft_f32(CFFT_imbalance_init_struct, (float*)&IQ_imbalance_cfft_input,0,1);

    GetSpecPwr (( COMPLEX_float*)&IQ_imbalance_cfft_input);

    UpdateIQParams (( COMPLEX_float*)&IQ_imbalance_cfft_input);

    CompIQ(( COMPLEX_float*)&IQ_imbalance_cfft_input);

    if (mode<=1)//Здесь дополнительная фильтрация ФВЧ
      arm_cmplx_mult_cmplx_f32((float*)&IQ_imbalance_cfft_input,
                             (float*)&HP_FILTER,
                             (float*)&IQ_imbalance_cfft_input,
                             (FFT_IQ_imbalanse_size));


    arm_cfft_f32(CFFT_imbalance_init_struct, (float*)&IQ_imbalance_cfft_input,1,1);

    copy_overlap_output_cfft_buffer(( COMPLEX_float*)&IQ_imbalance_cfft_input,
                                  ( COMPLEX_float*)&cfft_buffer[i*FFT_IQ_imbalanse_size/2]);

  }
}
