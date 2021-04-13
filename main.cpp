/**
 * This program initialize two arrays consist 1000 items double type and transfer them to python function, 
 * which is additiona and subtract items of 2 different c++ arrays and measures the time spent on work.
 * 
 * Input parametres of program is path to python file, name of python file without '.py' and function name.
 * 
 **/

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/python.hpp>


int main(int argc, const char** argv) {

    // Initialize array size
    const long unsigned int SIZE = 1000;
    
    // Initialize path to python file, name of python file without '.py' and function name.
    std::string py_path = argv[1];
    std::string py_filename = argv[2];
    std::string py_func_name = argv[3];

    // Initialize arrays and fill them with random numbers
    boost::array<double, SIZE> arr1;
    boost::array<double, SIZE> arr2;

    srand(time(NULL));

    for (auto i = 0; i < SIZE; i++)
    {
        arr1[i] = (double) (rand() % 100000 + 100) / 100.0;
        arr2[i] = (double) (rand() % 100000 + 100) / 100.0;
    }

    double* const x1 = &arr1[0];
    double* const x2 = &arr2[0];

    // Fixing start time of work
    auto start = std::chrono::high_resolution_clock::now();

    // Upload a python intepreter
    Py_Initialize();
    namespace bp = boost::python;

    // Initialize and fill python list with items from c++ arrays
    bp::list list1, list2;

    std::for_each(x1, x1 + SIZE, boost::bind(&bp::list::append<double>, &list1, _1));
    std::for_each(x2, x2 + SIZE, boost::bind(&bp::list::append<double>, &list2, _1));

    try
    {
        // Set a path to python file
        bp::object sys = bp::import("sys");
        bp::object sys_path = sys.attr("path");
        bp::object folder_path = sys_path.attr("append")(py_path.c_str());

        // Import python file and call function
        bp::object module = bp::import(py_filename.c_str());
        bp::object result = module.attr(py_func_name.c_str())(list1, list2);

        // Override python list with 2 elements of python tuple, wich consis result of work python function
        list1 = bp::extract<bp::list>(result[0]);
        list2 = bp::extract<bp::list>(result[1]);

        // Convert python values to c++ values and include it into arrays
        for (int i = 0; i < SIZE; i++)
        {
            arr1[i] = bp::extract<double>(list1[i]);
            arr2[i] = bp::extract<double>(list2[i]);
        }

    }
    catch(bp::error_already_set&)
    {
        PyErr_Print();
    }

    // Fixing end time of work python function
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_expiration = end - start;
    
    // Output time of work
    std::cout << "Time expiration: " << time_expiration.count() << std::endl;
    return 0;
}
