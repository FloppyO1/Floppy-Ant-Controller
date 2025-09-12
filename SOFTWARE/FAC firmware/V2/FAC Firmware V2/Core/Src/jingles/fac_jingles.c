/*
 * FAC_jingles.c
 *
 *  Created on: Sep 12, 2025
 *      Author: filippo-castellan
 */

#include "jingles/fac_jingles.h"
#include "jingles/notes.h"

void FAC_jingle_Tequila(){
	// "Tequila" riff
	FAC_motor_make_noise(NOTE_C6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	HAL_Delay(125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_G6, 125);

	HAL_Delay(125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	HAL_Delay(125);

	FAC_motor_make_noise(NOTE_C6, 125);

	HAL_Delay(625);

	FAC_motor_make_noise(NOTE_C6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	HAL_Delay(125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_G6, 125);

	HAL_Delay(125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	HAL_Delay(800);
}

void FAC_jingle_Tequila_long(){
	// "Tequila" riff
	FAC_motor_make_noise(NOTE_C6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	HAL_Delay(125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_G6, 125);

	HAL_Delay(125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	HAL_Delay(125);

	FAC_motor_make_noise(NOTE_C6, 125);

	HAL_Delay(625);

	FAC_motor_make_noise(NOTE_C6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	HAL_Delay(125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_G6, 125);

	HAL_Delay(125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	HAL_Delay(800);

	// Music Composer generator

	FAC_motor_make_noise(NOTE_C6, 125);
	FAC_motor_make_noise(NOTE_D6, 125);
	FAC_motor_make_noise(NOTE_F6, 125);
	FAC_motor_make_noise(NOTE_GS6, 250);
	FAC_motor_make_noise(NOTE_GS6, 250);
	FAC_motor_make_noise(NOTE_F6, 250);
	HAL_Delay(250);
	FAC_motor_make_noise(NOTE_C6, 125);
	FAC_motor_make_noise(NOTE_D6, 125);
	FAC_motor_make_noise(NOTE_F6, 125);
	FAC_motor_make_noise(NOTE_GS6, 250);
	FAC_motor_make_noise(NOTE_GS6, 250);
	FAC_motor_make_noise(NOTE_F6, 250);
	HAL_Delay(250);
	FAC_motor_make_noise(NOTE_G6, 125);
	FAC_motor_make_noise(NOTE_G6, 125);
	FAC_motor_make_noise(NOTE_G6, 125);
	FAC_motor_make_noise(NOTE_G6, 125);
	FAC_motor_make_noise(NOTE_G6, 250);
	FAC_motor_make_noise(NOTE_G6, 250);
	FAC_motor_make_noise(NOTE_C6, 250);
	HAL_Delay(250);
}

void FAC_jingle_neverGiveYouUp(){
	// Music Composer generator

	FAC_motor_make_noise(NOTE_A5, 125);
	FAC_motor_make_noise(NOTE_B5, 125);
	FAC_motor_make_noise(NOTE_C6, 125);
	FAC_motor_make_noise(NOTE_C6, 125);
	FAC_motor_make_noise(NOTE_D6, 250);
	FAC_motor_make_noise(NOTE_B5, 125);
	FAC_motor_make_noise(NOTE_A5, 125);
	FAC_motor_make_noise(NOTE_G5, 625);
	HAL_Delay(125);
	FAC_motor_make_noise(NOTE_A5, 250);
	FAC_motor_make_noise(NOTE_A5, 125);
	FAC_motor_make_noise(NOTE_B5, 125);
	FAC_motor_make_noise(NOTE_C6, 125);
	FAC_motor_make_noise(NOTE_A5, 125);
	FAC_motor_make_noise(NOTE_G5, 250);
	FAC_motor_make_noise(NOTE_G6, 250);
	FAC_motor_make_noise(NOTE_G6, 250);
	FAC_motor_make_noise(NOTE_D6, 250);
	HAL_Delay(250);
	HAL_Delay(250);
}
