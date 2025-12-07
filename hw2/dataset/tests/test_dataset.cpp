#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <vector>
#include <variant>
#include <filesystem>

#include "../dataset.hpp"

using namespace SciCpp;

static std::string make_temp_csv() {
    std::string path = "test_dataset_temp.csv";
    std::ofstream out(path);
    // header: numeric, string, numeric
    out << "x,y,z\n";
    out << "1.5,hello,2.0\n";
    out << "2.5,world,3.0\n";
    out.close();
    return path;
}

static std::string make_temp_csv_with_empty() {
    std::string path = "test_dataset_temp_empty.csv";
    std::ofstream out(path);
    out << "x,y,z\n";
    out << "1.0,,2.0\n"; // empty y
    out << "2.0,world,3.0\n";
    out.close();
    return path;
}

TEST(DatasetTest, ConstructAndSize) {
    auto path = make_temp_csv();
    // Construct dataset successfully and check size
    Dataset ds(path);
    EXPECT_EQ(ds.size(), 2u);
    std::filesystem::remove(path);
}

TEST(DatasetTest, RowAccessByIndex) {
    auto path = make_temp_csv();
    Dataset ds(path);

    auto row0 = ds(0);
    ASSERT_EQ(row0.size(), 3u);

    // cell 0 should be optional<double> with value 1.5
    EXPECT_TRUE(std::holds_alternative<std::optional<double>>(row0[0]));
    auto od0 = std::get<std::optional<double>>(row0[0]);
    ASSERT_TRUE(od0.has_value());
    EXPECT_DOUBLE_EQ(od0.value(), 1.5);

    // cell 1 should be optional<string> with value "hello"
    EXPECT_TRUE(std::holds_alternative<std::optional<std::string>>(row0[1]));
    auto os1 = std::get<std::optional<std::string>>(row0[1]);
    ASSERT_TRUE(os1.has_value());
    EXPECT_EQ(os1.value(), "hello");

    std::filesystem::remove(path);
}

TEST(DatasetTest, ColumnAccessByName) {
    auto path = make_temp_csv();
    Dataset ds(path);

    // Column "x"
    auto col_x = ds(std::string("x"));
    ASSERT_TRUE(std::holds_alternative<std::vector<std::optional<double>>>(col_x));
    auto vx = std::get<std::vector<std::optional<double>>>(col_x);
    ASSERT_EQ(vx.size(), 2u);
    ASSERT_TRUE(vx[0].has_value());
    EXPECT_DOUBLE_EQ(vx[0].value(), 1.5);
    ASSERT_TRUE(vx[1].has_value());
    EXPECT_DOUBLE_EQ(vx[1].value(), 2.5);

    // Column "y"
    auto col_y = ds(std::string("y"));
    ASSERT_TRUE(std::holds_alternative<std::vector<std::optional<std::string>>>(col_y));
    auto vy = std::get<std::vector<std::optional<std::string>>>(col_y);
    ASSERT_EQ(vy.size(), 2u);
    ASSERT_TRUE(vy[0].has_value());
    EXPECT_EQ(vy[0].value(), "hello");
    ASSERT_TRUE(vy[1].has_value());
    EXPECT_EQ(vy[1].value(), "world");

    std::filesystem::remove(path);
}

TEST(DatasetTest, CellAccessByIndexAndName) {
    auto path = make_temp_csv();
    Dataset ds(path);

    // Cell at (1, "y")
    auto c = ds(1, std::string("y"));
    EXPECT_TRUE(std::holds_alternative<std::optional<std::string>>(c));
    auto oc = std::get<std::optional<std::string>>(c);
    ASSERT_TRUE(oc.has_value());
    EXPECT_EQ(oc.value(), "world");

    std::filesystem::remove(path);
}

TEST(DatasetTest, Iteration) {
    auto path = make_temp_csv();
    Dataset ds(path);

    size_t count = 0;
    for (auto it = ds.begin(); it != ds.end(); ++it) {
        ++count;
    }
    EXPECT_EQ(count, ds.size());

    // range-based
    size_t rb_count = 0;
    for (const auto &r : ds) {
        (void)r;
        ++rb_count;
    }
    EXPECT_EQ(rb_count, ds.size());

    std::filesystem::remove(path);
}

TEST(DatasetTest, OptionalParsing) {
    auto path = make_temp_csv_with_empty();
    Dataset ds(path);

    // first row y is empty -> should parse to nullopt
    auto c0 = ds(0, std::string("y"));
    EXPECT_TRUE(std::holds_alternative<std::optional<std::string>>(c0));
    auto o0 = std::get<std::optional<std::string>>(c0);
    EXPECT_FALSE(o0.has_value());

    // second row y has value
    auto c1 = ds(1, std::string("y"));
    auto o1 = std::get<std::optional<std::string>>(c1);
    ASSERT_TRUE(o1.has_value());
    EXPECT_EQ(o1.value(), "world");

    // column access shows optional at index 0 is empty
    auto col_y = ds(std::string("y"));
    auto vy = std::get<std::vector<std::optional<std::string>>>(col_y);
    EXPECT_FALSE(vy[0].has_value());
    EXPECT_TRUE(vy[1].has_value());

    std::filesystem::remove(path);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}