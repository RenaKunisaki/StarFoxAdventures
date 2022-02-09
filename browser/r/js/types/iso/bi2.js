import Struct from '../../lib/Struct.js';

//bi2 country codes
export const CountryCodes = [
    "Japan",
    "Americas", //USA, Canada, others?
    "Europe",
    "All", //not valid on retail consoles
];

//Boot Info 2
export const Bi2Bin = Struct(
    ['I', 'debugMonitorSize'],
    ['I', 'simMemSize'],
    ['I', 'argOffs'],
    ['I', 'debugFlag'],
    ['I', 'trackLoc'],
    ['I', 'trackSize'],
    ['I', 'countryCode'],
    ['I', 'numDiscs'],
    ['I', 'supportLFN'],
    ['I', 'padSpec'],
    ['I', 'dolSizeLimit'],
);
