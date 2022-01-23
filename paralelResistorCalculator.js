let R_MAX = 5000;      // Ohms - max pot Value
let R_MIN = 1;          // Ohms - min pot Value
let I_DIODE = 0.015;    // Amps - current, that should pass trough diode
let U = 12;              // Volts - voltage in circuit
let U_DROP = 3.4;         // Volts - voltage drop of a diode
let DIODE_BRANCHES = 400; // Number of diode branches

let R_DIODE;            // Ohms - value of a resistor connected to each diode in series



// quadratic function to calculate R_DIODE

let a = ( I_DIODE * ( R_MIN + R_MAX ) );
let b = ( U_DROP * ( R_MIN + R_MAX ) + DIODE_BRANCHES * R_MAX * R_MIN * I_DIODE - U * ( 2 * R_MIN + R_MAX ) );
let c = ( DIODE_BRANCHES * R_MAX * R_MIN * ( U_DROP - 2 * U ) );

let res1 = (- b + Math.sqrt( b * b - 4 * a * c)) / (2 * a);
let res2 = (- b - Math.sqrt( b * b - 4 * a * c)) / (2 * a);


console.log(`Possible resistor values are ${res1} Ohms / ${res2} Ohms`);