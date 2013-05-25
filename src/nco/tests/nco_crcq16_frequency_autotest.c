/*
 * Copyright (c) 2007, 2008, 2009, 2010, 2011, 2012, 2013 Joseph Gaeddert
 *
 * This file is part of liquid.
 *
 * liquid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liquid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liquid.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <complex.h>
#include "autotest/autotest.h"
#include "liquid.h"

// declare external arrays
float complex nco_sincos_fsqrt1_2[];
float complex nco_sincos_fsqrt1_3[];
float complex nco_sincos_fsqrt1_5[];
float complex nco_sincos_fsqrt1_7[];

// autotest helper function
//  _type       :   NCO type (e.g. LIQUID_NCO)
//  _phase      :   initial phase
//  _frequency  :   initial frequency
//  _sincos     :   external sin/cosine data
//  _num_samples:   number of samples to test
//  _tol        :   error tolerance
void nco_crcq16_frequency_test(int             _type,
                               float           _phase,
                               float           _frequency,
                               float complex * _sincos,
                               unsigned int    _num_samples,
                               float           _tol)
{
    // create object
    nco_crcq16 nco = nco_crcq16_create(_type);

    // set phase and frequency
    nco_crcq16_set_phase(    nco, q16_angle_float_to_fixed(_phase)    );
    nco_crcq16_set_frequency(nco, q16_angle_float_to_fixed(_frequency));

    // run trials
    unsigned int i;
    for (i=0; i<_num_samples; i++) {
        // compute complex output
        cq16_t y_test;
        nco_crcq16_cexpf(nco, &y_test);

        // compare to expected output
        float complex y = _sincos[i];

        // run tests
        CONTEND_DELTA( q16_fixed_to_float(y_test.real), crealf(y), _tol );
        CONTEND_DELTA( q16_fixed_to_float(y_test.imag), cimagf(y), _tol );

        // step oscillator
        nco_crcq16_step(nco);
    }

    // destroy object
    nco_crcq16_destroy(nco);
}


// test floating point precision nco phase
void autotest_nco_crcq16_frequency()
{
    // error tolerance
    float tol = 0.055f;

    // test frequencies with irrational values
    nco_crcq16_frequency_test(LIQUID_NCO, 0.0f, 0.707106781186547, nco_sincos_fsqrt1_2, 256, tol); // 1/sqrt(2)
    nco_crcq16_frequency_test(LIQUID_NCO, 0.0f, 0.577350269189626, nco_sincos_fsqrt1_3, 256, tol); // 1/sqrt(3)
    nco_crcq16_frequency_test(LIQUID_NCO, 0.0f, 0.447213595499958, nco_sincos_fsqrt1_5, 256, tol); // 1/sqrt(5)
    nco_crcq16_frequency_test(LIQUID_NCO, 0.0f, 0.377964473009227, nco_sincos_fsqrt1_7, 256, tol); // 1/sqrt(7)
}

// test floating point precision vco phase
void autotest_vco_crcq16_frequency()
{
    // error tolerance
    float tol = 0.055f;

    // test frequencies with irrational values
    nco_crcq16_frequency_test(LIQUID_VCO, 0.0f, 0.707106781186547, nco_sincos_fsqrt1_2, 256, tol); // 1/sqrt(2)
    nco_crcq16_frequency_test(LIQUID_VCO, 0.0f, 0.577350269189626, nco_sincos_fsqrt1_3, 256, tol); // 1/sqrt(3)
    nco_crcq16_frequency_test(LIQUID_VCO, 0.0f, 0.447213595499958, nco_sincos_fsqrt1_5, 256, tol); // 1/sqrt(5)
    nco_crcq16_frequency_test(LIQUID_VCO, 0.0f, 0.377964473009227, nco_sincos_fsqrt1_7, 256, tol); // 1/sqrt(7)
}

