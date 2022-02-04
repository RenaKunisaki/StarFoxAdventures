/** Define some data types commonly used by the game.
 */
import Struct from '../lib/Struct.js';

export const Vec3f = Struct(
    ['f', 'x'],
    ['f', 'y'],
    ['f', 'z'],
);
export const Vec3i = Struct(
    ['f', 'x'],
    ['f', 'y'],
    ['f', 'z'],
);
