#include "swos.h"
#include "../stdinc.h"

// SWOS Random Number Generator implementation
// Based on the assembly code from the original SWOS game
namespace SWOS {

// The random table from the original assembly code
static const uint8_t kRandomTable[256] = {
    124, 154, 146, 70, 101, 250, 173, 89, 117, 26, 67, 12, 238, 147, 226, 34,
    78, 199, 253, 107, 125, 87, 170, 208, 188, 72, 5, 51, 224, 246, 145, 32,
    110, 234, 68, 43, 99, 39, 203, 86, 177, 163, 198, 162, 128, 16, 93, 255,
    148, 10, 59, 54, 175, 222, 202, 249, 127, 225, 172, 239, 8, 214, 142, 235,
    167, 73, 44, 7, 130, 105, 114, 97, 245, 159, 56, 197, 213, 221, 180, 129,
    106, 69, 179, 95, 90, 46, 103, 122, 216, 80, 45, 156, 18, 36, 134, 187,
    229, 186, 184, 144, 109, 150, 183, 135, 29, 251, 174, 71, 223, 244, 232, 83,
    81, 164, 25, 79, 62, 200, 65, 181, 132, 55, 241, 21, 252, 82, 178, 123,
    219, 227, 151, 76, 91, 28, 6, 254, 119, 231, 85, 14, 236, 185, 108, 24,
    111, 40, 98, 35, 168, 189, 41, 74, 102, 209, 96, 153, 176, 133, 247, 50,
    33, 52, 113, 141, 206, 42, 3, 160, 217, 161, 193, 64, 143, 205, 182, 233,
    139, 37, 237, 84, 131, 121, 4, 22, 20, 194, 243, 169, 0, 201, 48, 61,
    9, 212, 137, 57, 1, 126, 155, 92, 31, 195, 19, 88, 228, 116, 15, 94,
    191, 210, 47, 60, 218, 115, 240, 118, 158, 104, 138, 171, 13, 120, 211, 27,
    157, 242, 66, 207, 152, 30, 2, 140, 196, 17, 190, 149, 75, 230, 38, 11,
    77, 100, 192, 220, 204, 166, 165, 53, 112, 136, 23, 63, 49, 248, 215, 58
};

// Global variables for the random number generator
static uint8_t seed = 0;
static uint8_t randXorIndex = 0;
static uint8_t randXorKey = 0;

// SWOS random number generator function
// This implements the algorithm from the original assembly code
int rand()
{
    uint8_t d0 = seed;
    
    if (d0 == 0) {
        // If seed is zero, increment randXorIndex and use it to get a new key
        randXorIndex++;
        uint8_t d1 = randXorIndex;
        randXorKey = kRandomTable[d1];
    }
    
    // Get value from table using current seed
    uint8_t tableValue = kRandomTable[d0];
    
    // XOR with the stored key
    uint8_t result = tableValue ^ randXorKey;
    
    // Increment seed for next call
    seed++;
    
    return result;
}

// Function to initialize the random number generator
void srand(uint8_t newSeed)
{
    seed = newSeed;
}

} // namespace SWOS