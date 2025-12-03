#pragma once

#include <string>
#include <vector>
#include <variant>

enum class ColumnType {
    Double,
    String
};

class Dataset {
public:
    using Cell = std::variant<double, std::string>;
    using Column = std::variant<std::vector<double>, std::vector<std::string>>;
    using Row = std::vector<Cell>;

    // Build a dataset object from a csv file path
    Dataset(const std::string& path);
    // Get the number of samples in the dataset
    size_t size() const;

    // Override the () operator to access samples by index and column name
    Row operator()(size_t index) const;
    Column operator()(const std::string& column_name) const;
    Cell operator()(size_t index, const std::string& column_name) const;

    // Iterators to traverse the dataset
    std::vector<Row>::const_iterator begin() const;
    std::vector<Row>::const_iterator end() const;

private:
    std::vector<std::string> column_names;
    std::vector<ColumnType> column_types;
    std::vector<Column> columns;
    size_t num_rows;
};