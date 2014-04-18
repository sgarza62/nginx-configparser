#include "gtest/gtest.h"
#include "config_parser.h"

// Test fixture for parsing config files
class NginxFileConfigTest : public ::testing::Test{
protected:
  bool ParseFile(const char* file_name) {
    bool success = parser.Parse(file_name, &out_config);
    return success;
  }
  NginxConfigParser parser;
  NginxConfig out_config;
};

// Test valid config files
TEST_F(NginxFileConfigTest, SimpleConfig) {
  EXPECT_TRUE(ParseFile("test_config_files/example_file"));
}
TEST_F(NginxFileConfigTest, TypicalConfig) {
  EXPECT_TRUE(ParseFile("test_config_files/typical_config"));
}
TEST_F(NginxFileConfigTest, ComplexConfig) {
  EXPECT_TRUE(ParseFile("test_config_files/complex_config"));
}
TEST_F(NginxFileConfigTest, OneLineConfig) {
  EXPECT_TRUE(ParseFile("test_config_files/one_line_config_file"));
}

// Test invalid config files
TEST_F(NginxFileConfigTest, InvalidOneLineConfigs) {
  // These files are one-liners that contain comments, midway through
  // the line, thus preventing closing braces from being parsed.
  // Parser reaches EOF before finding a closing brace, and should
  // return false.
  // event { # ... } EOF
  EXPECT_FALSE(ParseFile("test_config_files/invalid_one_line_config_with_comment_1"));
  EXPECT_FALSE(ParseFile("test_config_files/invalid_one_line_config_with_comment_2"));
}

TEST_F(NginxFileConfigTest, ConfigWithUnclosedBlock) {
  EXPECT_FALSE(ParseFile("test_config_files/invalid_unclosed_block_config"));
}





TEST(NginxConfigStatementTest, ToString) {
  NginxConfigStatement statement;
  statement.tokens_.push_back("foo");
  statement.tokens_.push_back("bar");
  EXPECT_EQ("foo bar;\n", statement.ToString(0));
}

class NginxStringConfigTest : public ::testing::Test{
protected:
  bool ParseString(const std::string& config_string) {
    std::stringstream config_stream(config_string);
    return parser_.Parse(&config_stream, &out_config_);
  }
  NginxConfigParser parser_;
  NginxConfig out_config_;
};

TEST_F(NginxStringConfigTest, SimpleConfig) {
  EXPECT_TRUE(ParseString("foo bar;"));
  EXPECT_EQ(1, out_config_.statements_.size());
}

TEST_F(NginxStringConfigTest, ConfigWithBlock) {
  EXPECT_TRUE(ParseString("server {listen 80; }"));
}

TEST_F(NginxStringConfigTest, NestedConfig) {
  EXPECT_TRUE(ParseString("foo { bar { baz aux; } } "));
}


