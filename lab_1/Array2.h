#ifndef ARRAY2_H
#define ARRAY2_H

#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

template <typename T>
class Array2 {
private:
    vector<vector<T>> data; // Stores the elements of the array
    size_t width;  // Width of the array
    size_t height; // Height of the array

public:
    // Default constructor (creates an empty array)
    Array2() : width(0), height(0) {}

    // Constructor with parameters to set the dimensions of the array
    Array2(size_t w, size_t h) : width(w), height(h) {
        data.resize(height, vector<T>(width)); // Resize the vector to the specified dimensions
    }

    // Function to get the width of the array
    size_t getWidth() const {
        return width;
    }

    // Function to get the height of the array
    size_t getHeight() const {
        return height;
    }

    // Function to get the total number of elements in the array
    size_t getSize() const {
        return width * height; // Return the product of width and height
    }

    // Function call operator for accessing the (x, y) element
    T& operator()(size_t x, size_t y) {
        // Check for out-of-bounds access
        if (x >= width || y >= height) {
            throw out_of_range("Index out of range"); // Throw an exception if indices are out of range
        }
        return data[y][x]; // Return a reference to the specified element
    }

    // Const version of the function call operator for read-only access
    const T& operator()(size_t x, size_t y) const {
        // Check for out-of-bounds access
        if (x >= width || y >= height) {
            throw out_of_range("Index out of range"); // Throw an exception if indices are out of range
        }
        return data[y][x]; // Return a const reference to the specified element
    }

    // Stream insertion operator for outputting the array to a stream
    friend ostream& operator<<(ostream& os, const Array2<T>& array) {
        for (size_t y = 0; y < array.height; ++y) {
            for (size_t x = 0; x < array.width; ++x) {
                os << array.data[y][x] << " "; // Output each element followed by a space
            }
            os << endl; // End the line after each row
        }
        return os; // Return the output stream
    }

    // Stream extraction operator for reading the array from a stream
    friend istream& operator>>(istream& is, Array2<T>& array) {
        for (size_t y = 0; y < array.height; ++y) {
            for (size_t x = 0; x < array.width; ++x) {
                is >> array.data[y][x]; // Read each element from the input stream
            }
        }
        return is; // Return the input stream
    }
};

#endif // ARRAY2_H
