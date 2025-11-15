/*
 * FAC_jingles.c
 *
 *  Created on: Sep 12, 2025
 *      Author: filippo-castellan
 */

#include "jingles/fac_jingles.h"
#include "jingles/notes.h"
#include "iwdg.h"

/*
 * @brief	Delay with watchdog, otherwise it will reset the MCU
 * @param1	Duration of the wait in milliseconds
 * @notes	Delays must be multiple of 10ms
 */
static void FAC_jingles_delay(uint16_t ms) {
	for (uint16_t i = 0; i < ms/10; i++) {
		HAL_Delay(10);
		HAL_IWDG_Refresh(&hiwdg);
	}
}

void FAC_jingle_Tequila() {
	// "Tequila" riff
	FAC_motor_make_noise(NOTE_C6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_jingles_delay(125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_G6, 125);

	FAC_jingles_delay(125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_jingles_delay(125);

	FAC_motor_make_noise(NOTE_C6, 125);

	FAC_jingles_delay(625);

	FAC_motor_make_noise(NOTE_C6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_jingles_delay(125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_G6, 125);

	FAC_jingles_delay(125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_jingles_delay(800);
}

void FAC_jingle_Tequila_long() {
	// "Tequila" riff
	FAC_motor_make_noise(NOTE_C6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_jingles_delay(125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_G6, 125);

	FAC_jingles_delay(125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_jingles_delay(125);

	FAC_motor_make_noise(NOTE_C6, 125);

	FAC_jingles_delay(625);

	FAC_motor_make_noise(NOTE_C6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_jingles_delay(125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_G6, 125);

	FAC_jingles_delay(125);

	FAC_motor_make_noise(NOTE_DS6, 125);

	FAC_motor_make_noise(NOTE_F6, 125);

	FAC_jingles_delay(800);

	// Music Composer generator

	FAC_motor_make_noise(NOTE_C6, 125);
	FAC_motor_make_noise(NOTE_D6, 125);
	FAC_motor_make_noise(NOTE_F6, 125);
	FAC_motor_make_noise(NOTE_GS6, 250);
	FAC_motor_make_noise(NOTE_GS6, 250);
	FAC_motor_make_noise(NOTE_F6, 250);
	FAC_jingles_delay(250);
	FAC_motor_make_noise(NOTE_C6, 125);
	FAC_motor_make_noise(NOTE_D6, 125);
	FAC_motor_make_noise(NOTE_F6, 125);
	FAC_motor_make_noise(NOTE_GS6, 250);
	FAC_motor_make_noise(NOTE_GS6, 250);
	FAC_motor_make_noise(NOTE_F6, 250);
	FAC_jingles_delay(250);
	FAC_motor_make_noise(NOTE_G6, 125);
	FAC_motor_make_noise(NOTE_G6, 125);
	FAC_motor_make_noise(NOTE_G6, 125);
	FAC_motor_make_noise(NOTE_G6, 125);
	FAC_motor_make_noise(NOTE_G6, 250);
	FAC_motor_make_noise(NOTE_G6, 250);
	FAC_motor_make_noise(NOTE_C6, 250);
	FAC_jingles_delay(250);
}

void FAC_jingle_neverGiveYouUp() {
	// Music Composer generator

	FAC_motor_make_noise(NOTE_A5, 125);
	FAC_motor_make_noise(NOTE_B5, 125);
	FAC_motor_make_noise(NOTE_C6, 125);
	FAC_motor_make_noise(NOTE_C6, 125);
	FAC_motor_make_noise(NOTE_D6, 250);
	FAC_motor_make_noise(NOTE_B5, 125);
	FAC_motor_make_noise(NOTE_A5, 125);
	FAC_motor_make_noise(NOTE_G5, 625);
	FAC_jingles_delay(125);
	FAC_motor_make_noise(NOTE_A5, 250);
	FAC_motor_make_noise(NOTE_A5, 125);
	FAC_motor_make_noise(NOTE_B5, 125);
	FAC_motor_make_noise(NOTE_C6, 125);
	FAC_motor_make_noise(NOTE_A5, 125);
	FAC_motor_make_noise(NOTE_G5, 250);
	FAC_motor_make_noise(NOTE_G6, 250);
	FAC_motor_make_noise(NOTE_G6, 250);
	FAC_motor_make_noise(NOTE_D6, 250);
	FAC_jingles_delay(250);
	FAC_jingles_delay(250);
}

