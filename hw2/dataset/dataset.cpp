#include "file_handler.hpp"
#include <fstream>
#include <sstream>
#include <cctype>

namespace scmp {

Cell DataTable::parse_cell(const std::string& token) {
    if (token.empty() || token == "NA")
        return std::nullopt;

    // Try to parse as number
    try {
        size_t idx;
        double val = std::stod(token, &idx);
        if (idx == token.size())
            return val;
    } catch (...) {}

    // Otherwise treat as string
    return std::string(token);
}

std::vector<std::string> DataTable::split_csv_line(const std::string& line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ',')) {
        result.push_back(token);
    }
    return result;
}

DataTable DataTable::from_csv(const std::string& filename) {
    DataTable table;
    std::ifstream f(filename);

    if (!f)
        throw std::runtime_error("Cannot open CSV file: " + filename);

    std::string line;
    while (std::getline(f, line)) {
        auto tokens = split_csv_line(line);

        Row row;
        row.reserve(tokens.size());

        for (auto& tk : tokens)
            row.push_back(parse_cell(tk));

        table.rows_.push_back(std::move(row));
    }

    return table;
}

// NOTE: This is a *minimal* JSON loader.
// It expects a structure like:
// [ ["1","2","3"], ["4","5","6"] ]
DataTable DataTable::from_json(const std::string& filename) {
    std::ifstream f(filename);
    if (!f)
        throw std::runtime_error("Cannot open JSON file: " + filename);

    std::stringstream buffer;
    buffer << f.rdbuf();
    std::string json = buffer.str();

    DataTable table;
    Row row;
    std::string token;
    bool in_string = false;

    for (size_t i = 0; i < json.size(); ++i) {
        char c = json[i];

        if (c == '"') {
            in_string = !in_string;
            continue;
        }

        if (in_string) {
            token.push_back(c);
            continue;
        }

        if (c == ',' || c == ']') {
            if (!token.empty()) {
                row.push_back(parse_cell(token));
                token.clear();
            }
            if (c == ']' && !row.empty()) {
                table.rows_.push_back(row);
                row.clear();
            }
        }
    }

    return table;
}

} // namespace scmp
