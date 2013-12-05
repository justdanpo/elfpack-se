/*************************************************************************
Cephes Math Library Release 2.8:  June, 2000
Copyright by Stephen L. Moshier

Contributors:
    * Sergey Bochkanov (ALGLIB project). Translation from C to
      pseudocode.

See subroutines comments for additional copyrights.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

- Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer listed
  in this license in the documentation and/or other materials
  provided with the distribution.

- Neither the name of the copyright holders nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************/

#ifndef _gammaf_h
#define _gammaf_h


/*************************************************************************
Вычисление гамма-функции.

Входные параметры:
    X   -   аргумент

Допустимые значения:
    0 < X < 171.6
    -170 < X < 0, X не целое.

Относительная погрешность вычисления:

    ОБЛАСТЬ         #ТЕСТОВ        МАКС.          СРЕДН.
    (-170, -33)       20000      2.3e-15         3.3e-16
    (-33,   33)       20000      9.4e-16         2.2e-16
    ( 33,  171.6)     20000      2.3e-15         3.2e-16

Cephes Math Library Release 2.8:  June, 2000
Original copyright 1984, 1987, 1989, 1992, 2000 by Stephen L. Moshier
Translated to AlgoPascal by Bochkanov Sergey (2005, 2006).
*************************************************************************/
double gamma(double x);


/*************************************************************************
Вычисление натурального логарифма модуля гамма-функции.

Входные параметры:
    x       -   точка, в которой вычисляем логарифм:

Выходные параметры:
    SgnGam  -   знак, который имеет гамма-функция.
                Либо +1, либо -1.

Результат:
    натуральный логарифм модуля гамма-функции

Допустимые значения:
    0 < X < 2.55e305
    -2.55e305 < X < 0, X не целое.

Погрешность аппроксимации (относительная для значений функции
больших 1, иначе - абсолютная):

domain               #trials   peak        rms
0, 3                 28000     5.4e-16     1.1e-16
2.718, 2.556e305     40000     3.5e-16     8.3e-17

Следующий тест приводит относительную погрешность, хотя для некоторых
точек ошибка может быть больше, чем указано в таблице:
-200, -4             10000     4.8e-16     1.3e-16

Cephes Math Library Release 2.8:  June, 2000
Copyright 1984, 1987, 1989, 1992, 2000 by Stephen L. Moshier
Translated to AlgoPascal by Bochkanov Sergey (2005, 2006).
*************************************************************************/
double lngamma(double x, double& sgngam);


#endif
