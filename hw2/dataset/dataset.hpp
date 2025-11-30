#pragma once

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <stdexcept>

namespace scmp { // scientific computing

using Cell = std::optional<std::variant<double, std::string>>;

class DataTable {
public:
    using Row = std::vector<Cell>;
    using Iterator = std::vector<Row>::iterator;
    using ConstIterator = std::vector<Row>::const_iterator;

    DataTable() = default;

    // Loaders
    static DataTable from_csv(const std::string& filename);
    static DataTable from_json(const std::string& filename);

    // Access
    const Row& row(size_t i) const { return rows_.at(i); }
    size_t nrows() const { return rows_.size(); }
    size_t ncols() const { return rows_.empty() ? 0 : rows_[0].size(); }

    // Iterators
    Iterator begin() { return rows_.begin(); }
    Iterator end() { return rows_.end(); }
    ConstIterator begin() const { return rows_.begin(); }
    ConstIterator end() const { return rows_.end(); }

private:
    std::vector<Row> rows_;

    static Cell parse_cell(const std::string& token);
    static std::vector<std::string> split_csv_line(const std::string& line);
};

} // namespace scmp
