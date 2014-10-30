/**
 * (c) Real-time systems project seminar, TU Darmstadt
 *
 * @file mysin.c
 * @author TEAM::GPS
 * @brief contains a lookup table of sinus values and functions to get sinus and cosinus
 * 			all values are normed to uint16_t
 *
 **/

#include "trigonometry.h"

static const uint16_t sinus[64] = {
	32767,
	35979,
	39160,
	42279,
	45306,
	48213,
	50971,
	53554,
	55937,
	58096,
	60012,
	61665,
	63040,
	64123,
	64904,
	65376,
	65534,
	65376,
	64904,
	64123,
	63040,
	61665,
	60012,
	58096,
	55937,
	53554,
	50971,
	48213,
	45306,
	42279,
	39160,
	35979,
	32767,
	29555,
	26374,
	23255,
	20228,
	17321,
	14563,
	11980,
	9597,
	7438,
	5522,
	3869,
	2494,
	1411,
	630,
	158,
	0,
	158,
	630,
	1411,
	2494,
	3869,
	5522,
	7438,
	9597,
	11980,
	14563,
	17321,
	20228,
	23255,
	26374,
	29555
};
uint16_t sin_(uint16_t i) {
	uint8_t index = i >> 10;
	return sinus[index];
}

uint16_t cos_(uint16_t i) {
	uint8_t index = i >> 10 ;
	index = (index + 16)%64;
	return sinus[index];
}

int16_t uint16AngleToDegree(uint16_t angle_in) {
	int32_t temp = (int32_t) angle_in;
	temp = 180 * (temp - UINT16_MAX / 2);
	return (int16_t) (temp / UINT16_MAX);
}

uint16_t AnglePlus(uint16_t angle_in, uint16_t plus){
	if (angle_in < (65535 - plus)) return angle_in + plus;
	else return  plus - (65535 - angle_in);
}

uint16_t AngleMinus(uint16_t angle_in, uint16_t minus){
	if (angle_in > minus) return angle_in - minus;
	else return 65535 - (minus-angle_in);
}

/*
 * Berechnet die Orientierung bezogen auf die x-Achse in Grad
 * @param Winkel in Weltkoordinaten
 * @return Winekl in Grad [-180..180]
 */
int16_t uint16DegreeRelativeToX(uint16_t angle_in) {
	int32_t temp = (int32_t) angle_in;
	temp = AngleMinus(angle_in, 16383);
	temp = 360 * (temp  - UINT16_MAX / 2);
	return (int16_t) (temp / UINT16_MAX);
}

/*
 *
 * @param Winkel in Weltkoordinaten
 * @return 0..360
 */
int angleToDegree(int angle_in){
	int temp;
	temp = 360*angle_in/65535;
	return temp;
}

/*
 * Winkel in Grad umrechnung in Kartenkoordiaten
 * @param Winkel in Grad
 * @return Winkel in Kartenkoordinaten
 */
uint16_t DegreeToUint16(int16_t angle_in){
	if (angle_in >= 0){
		angle_in = angle_in % 360;
		angle_in = angle_in * 182; // 182 = 65535/360
	} else {
		angle_in = angle_in % 360;
		angle_in = 65535 - 359 + angle_in*182;
	}
	angle_in = AngleMinus(angle_in, 16383);
	return (int)angle_in;
}

/**
 * Taylorapproximation der Cosinusfunktion
 * @param Winkel in Grad
 * @return Grad*100
 */
int32_t cos_Taylor(int32_t in){
	//Intern wird wird in rad gerechnet
	double help = 100*1 - (100*pow(3.1415*(in)/180,2))/2 + (100*pow(3.1415*(in)/180,4))/24 - (100*pow(3.1415*(in)/180, 6))/720 + (100*pow(3.1415*(in)/180, 8))/40320; // - (100*pow(3*(in)/180,10))/3628800;
	return (int32_t) help;
}

/**
 * Taylorapproximation der Sinusfunktion
 * @param Winkel in Grad
 * @return Grad*100
 */
int32_t sin_Taylor(int32_t in){
	double help = (100*3.1415*(in))/180 - (100*pow(3.1415*(in)/180,3))/6 + (100*pow(3.1415*(in)/180,5))/120 - (100*pow(3.1415*(in)/180, 7))/5040; // + (100*pow(3*(in)/180, 9))/362880;
	return (int32_t) help;
}
