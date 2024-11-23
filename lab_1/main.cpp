#include "Array2.h"
#include <cmath>  // For rounding

int main() {
    // Step 1: Read a double array from the input (including its dimensions)
    size_t width, height;

    // Get the dimensions of the array
    cout << "Enter the width and height of the array: ";
    cin >> width >> height;

    // Create an Array2<double> object with the provided dimensions
    Array2<double> doubleArray(width, height);

    // Input the elements of the Array2<double> from the standard input
    cout << "Enter elements for the double array (" << width << "x" << height << "):" << endl;
    cin >> doubleArray;

    // Step 2: Copy elements from Array2<double> to Array2<int> with rounding
    Array2<int> intArray(width, height); // Create Array2<int> with the same dimensions

    // Loop through the elements, round them, and copy to the intArray
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            intArray(x, y) = static_cast<int>(round(doubleArray(x, y))); // Round and convert to int
        }
    }

    // Step 3: Output the rounded int array
    cout << "Rounded int array:" << endl;
    cout << intArray; // Output the Array2<int>

    return 0;
}
