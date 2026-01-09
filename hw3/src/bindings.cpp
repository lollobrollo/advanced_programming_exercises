// pybind documentation was pretty usefol here:
// https://pybind11.readthedocs.io/en/stable/advanced/functions.html

#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // required for std::vector, std::optional, std::variant, std::map
#include <pybind11/functional.h>

#include "dataset/dataset.hpp"
#include "statistics/statistics.hpp"
#include "integration/integration.hpp"
#include "integration/midpoint_rule.hpp"
#include "integration/trapezoidal_rule.hpp"
#include "integration/simpsons_rule.hpp"
#include "integration/gaussian_quadrature.hpp"

namespace py = pybind11;
using namespace SciCpp;

// thanks to template metaprogramming, many function arguments are automatically inferrred

PYBIND11_MODULE(scicpp, m) {
    m.doc() = "Scientific Computing Toolbox Core Bindings";

    // --- 1. Dataset Bindings ---
    py::class_<Dataset> dataset(m, "Dataset");
    dataset.def(py::init<const std::string&>(), py::arg("path"))
           .def("size", &Dataset::size)
           // exposing the overloaded () operators renaming them, instead of casting to function pointers (method shown in lesson 12)
           .def("get_row", [](const Dataset &d, size_t idx) { return d(idx); })
           .def("get_column", [](const Dataset &d, const std::string &name) { return d(name); })
           .def("get_cell", [](const Dataset &d, size_t idx, const std::string &name) { return d(idx, name); });

    // --- 2. Statistics Bindings ---
    py::class_<StatisticalAnalyzer>(m, "StatisticalAnalyzer")
        .def(py::init<const Dataset&>(), py::keep_alive<1, 2>()) // Keep dataset alive as long as analyzer exists
        .def("mean", &StatisticalAnalyzer::mean)
        .def("median", &StatisticalAnalyzer::median)
        .def("variance", &StatisticalAnalyzer::variance)
        .def("std_deviation", &StatisticalAnalyzer::std_deviation)
        .def("correlation", &StatisticalAnalyzer::correlation)
        .def("frequency_count", &StatisticalAnalyzer::frequency_count)
        .def("generate_report", &StatisticalAnalyzer::generate_report);

    // --- 3. Integration Strategy (Polymorphism) ---
    py::class_<IntegrationStrategy>(m, "IntegrationStrategy");
    
    py::class_<MidpointRule, IntegrationStrategy>(m, "MidpointRule")
        .def(py::init<>());

    py::class_<TrapezoidalRule, IntegrationStrategy>(m, "TrapezoidalRule")
        .def(py::init<>());

    py::class_<SimpsonsRule, IntegrationStrategy>(m, "SimpsonsRule")
        .def(py::init<>());

    py::class_<GaussianQuadrature, IntegrationStrategy>(m, "GaussianQuadrature")
        .def(py::init<>());

    // --- 4. Integral Evaluator ---
    py::class_<IntegralEvaluator>(m, "IntegralEvaluator")
        /* 
         * While returning unique pointers in this way is allowed, it is illegal to
         * use them as function arguments. [...] The above signature would imply that 
         * Python needs to give up ownership of an object that is passed to this function, 
         * which is generally not possible.
         *
         * See: https://stackoverflow.com/questions/77564007/how-to-deal-with-libraries-requiring-unique-ptr-as-inputs-in-pybind11 
         */
        .def(py::init([](std::string expr, IntegrationStrategy* strategy) {
            return new IntegralEvaluator(expr, std::unique_ptr<IntegrationStrategy>(strategy));
        }))
        .def("__call__", &IntegralEvaluator::operator(), 
             py::arg("a"), py::arg("b"), py::arg("n_intervals"));
}