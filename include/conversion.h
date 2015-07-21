/**
 * @file
 * Conversion functions
 */

#ifndef CONVERSION_H
#define CONVERSION_H

#include <stdio.h>
#include "structures.h"

int lk_to_bc(LKM2 lk_m2, Skin *skins, BCM2 *ptr);
void analyze_events(BCM2 model);

#endif
