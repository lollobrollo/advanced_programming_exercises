#include "dataset.hpp"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <optional>

namespace SciCpp {

static std::vector<std::string> split_csv_line(const std::string &line) {
    if (line.empty()) return {};
    std::vector<std::string> out;
    size_t start = 0;
    size_t pos = std::string::npos;
    do {
        pos = line.find(',', start);
        if (pos == std::string::npos) {
            out.emplace_back(line.substr(start));
        } else {
            out.emplace_back(line.substr(start, pos - start));
            start = pos + 1;
        }
    } while (pos != std::string::npos);
    return out;
}

Dataset::Dataset(const std::string &path) : num_rows(0) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("could not open dataset file");

    std::string line;
    // read header
    if (!std::getline(in, line)) return; // empty file -> empty dataset
    column_names = split_csv_line(line);
    size_t ncols = column_names.size();
    
    std::vector<std::vector<std::string>> raw_rows;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        auto toks = split_csv_line(line);
        // If row has fewer columns, pad with empty strings
        if (toks.size() < ncols) toks.resize(ncols, "");
        raw_rows.push_back(std::move(toks));
    }

    num_rows = raw_rows.size();

    // detect column types (default to string)
    column_types.resize(ncols, ColumnType::String);
    for (size_t c = 0; c < ncols; ++c) {
        bool all_nonempty_parse_double = true;
        bool found_nonempty = false;
        for (size_t r = 0; r < num_rows; ++r) {
            const auto &s = raw_rows[r][c];
            if (s.empty()) continue; // empty fields considered as optional
            found_nonempty = true;
            try {
                size_t idx = 0;
                std::stod(s, &idx);
                if (idx != s.size()) { all_nonempty_parse_double = false; break; }
            } catch (...) {
                all_nonempty_parse_double = false; break;
            }
        }
        // If there are no non-empty entries, treat as string
        column_types[c] = (found_nonempty && all_nonempty_parse_double) ? ColumnType::Double : ColumnType::String;
    }

    // build columns
    columns.clear();
    columns.reserve(ncols);
    for (size_t c = 0; c < ncols; ++c) {
        if (column_types[c] == ColumnType::Double) {
            std::vector<std::optional<double>> col; col.reserve(num_rows);
            for (size_t r = 0; r < num_rows; ++r) {
                const auto &s = raw_rows[r][c];
                if (s.empty()) col.push_back(std::nullopt);
                else col.push_back(std::optional<double>(std::stod(s)));
            }
            columns.push_back(std::move(col));
        } else {
            std::vector<std::optional<std::string>> col; col.reserve(num_rows);
            for (size_t r = 0; r < num_rows; ++r) {
                const auto &s = raw_rows[r][c];
                if (s.empty()) col.push_back(std::nullopt);
                else col.push_back(std::optional<std::string>(s));
            }
            columns.push_back(std::move(col));
        }
    }

    // build rows (stored twice, but simplifies access access patterns)
    rows.clear();
    rows.reserve(num_rows);
    for (size_t r = 0; r < num_rows; ++r) {
        Row row; row.reserve(ncols);
        for (size_t c = 0; c < ncols; ++c) {
            if (column_types[c] == ColumnType::Double) {
                const auto &opt = std::get<std::vector<std::optional<double>>>(columns[c])[r];
                row.emplace_back(opt);
            } else {
                const auto &opt = std::get<std::vector<std::optional<std::string>>>(columns[c])[r];
                row.emplace_back(opt);
            }
        }
        rows.push_back(std::move(row));
    }
}

size_t Dataset::size() const { return num_rows; }

Dataset::Row Dataset::operator()(size_t index) const {
    if (index >= num_rows) throw std::out_of_range("row index out of range");
    return rows[index];
}

Dataset::Column Dataset::operator()(const std::string &column_name) const {
    auto it = std::find(column_names.begin(), column_names.end(), column_name);
    if (it == column_names.end()) throw std::out_of_range("column not found");
    size_t idx = std::distance(column_names.begin(), it);
    return columns[idx];
}

Dataset::Cell Dataset::operator()(size_t index, const std::string &column_name) const {
    if (index >= num_rows) throw std::out_of_range("row index out of range");
    auto it = std::find(column_names.begin(), column_names.end(), column_name);
    if (it == column_names.end()) throw std::out_of_range("column not found");
    size_t idx = std::distance(column_names.begin(), it);
    return rows[index][idx];
}

std::vector<Dataset::Row>::const_iterator Dataset::begin() const { return rows.cbegin(); }
std::vector<Dataset::Row>::const_iterator Dataset::end() const { return rows.cend(); }

} // namespace SciCpp
