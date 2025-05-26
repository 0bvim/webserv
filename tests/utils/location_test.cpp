#include "Location.hpp"
#include "parser.hpp"
#include "size.hpp"
#include <gtest/gtest.h>
#include <ios>
#include <limits>
#include <string>

std::string location_uri = "/";

TEST(Location, setLocationURIbyBuffer) {
	std::string buffer = "location " + location_uri + "{}";
	Location location(buffer);

	EXPECT_EQ(location.getURI(), location_uri);
}

TEST(Location, setLocationURIbyMethod) {
	Location location;
	location.setURI(location_uri);

	EXPECT_EQ(location.getURI(), location_uri);
}

std::string location_max_body_size_0 = "0";
size_t location_max_body_size_0_result = std::numeric_limits<size_t>::max();

TEST(Location, setLocationMaxBodySize0ByBuffer) {
	std::string buffer = "location /{client_max_body_size " + location_max_body_size_0 + ";}";
	Location location(buffer);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_0_result);
}

TEST(Location, setLocationMaxBodySize0ByMethod) {
	Location location;
	location.setMaxBodySize(location_max_body_size_0);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_0_result);
}

std::string location_max_body_size_10 = "10";
size_t location_max_body_size_10_result = 10;

TEST(Location, setLocationMaxBodySize10ByBuffer) {
	std::string buffer = "location /{client_max_body_size " + location_max_body_size_10+ ";}";
	Location location(buffer);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_10_result);
}

TEST(Location, setLocationMaxBodySize10ByMethod) {
	Location location;
	location.setMaxBodySize(location_max_body_size_10);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_10_result);
}

std::string location_max_body_size_byte = "1B";
size_t location_max_body_size_byte_result = size::BYTE;

TEST(Location, setLocationMaxBodySize1BByBuffer) {
	std::string buffer = "location /{client_max_body_size " + location_max_body_size_byte + ";}";
	Location location(buffer);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_byte_result);
}

TEST(Location, setLocationMaxBodySize1BByMethod) {
	Location location;
	location.setMaxBodySize(location_max_body_size_byte);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_byte_result);
}

std::string location_max_body_size_kilobyte = "1K";
size_t location_max_body_size_kilobyte_result = size::KILOBYTE;

TEST(Location, setLocationMaxBodySize1KByBuffer) {
	std::string buffer = "location /{client_max_body_size " + location_max_body_size_kilobyte+ ";}";
	Location location(buffer);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_kilobyte_result);
}

TEST(Location, setLocationMaxBodySize1KByMethod) {
	Location location;
	location.setMaxBodySize(location_max_body_size_kilobyte);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_kilobyte_result);
}

std::string location_max_body_size_megabyte = "1M";
size_t location_max_body_size_megabyte_result = size::MEGABYTE;

TEST(Location, setLocationMaxBodySize1MByBuffer) {
	std::string buffer = "location /{client_max_body_size " + location_max_body_size_megabyte + ";}";
	Location location(buffer);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_megabyte_result);
}

TEST(Location, setLocationMaxBodySize1MByMethod) {
	Location location;
	location.setMaxBodySize(location_max_body_size_megabyte);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_megabyte_result);
}

std::string location_max_body_size_gigabyte = "1G";
size_t location_max_body_size_gigabyte_result = size::GIGABYTE;

TEST(Location, setLocationMaxBodySize1GByBuffer) {
	std::string buffer = "location /{client_max_body_size " + location_max_body_size_gigabyte + ";}";
	Location location(buffer);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_gigabyte_result);
}

TEST(Location, setLocationMaxBodySize1GByMethod) {
	Location location;
	location.setMaxBodySize(location_max_body_size_gigabyte);

	EXPECT_EQ(location.getMaxBodySize(), location_max_body_size_gigabyte_result);
}

std::string location_root = "./var/www/html";

TEST(Location, setLocationRootByBuffer) {
	std::string buffer = "location /{root " + location_root + ";}";
	Location location(buffer);

	EXPECT_EQ(location.getRoot(), location_root);
}

TEST(Location, setLocationRootByMethod) {
	Location location;
	location.setRoot(location_root);

	EXPECT_EQ(location.getRoot(), location_root);
}

std::set<std::string> location_index = {"index.php"};

TEST(Location, setLocationIndexByBuffer) {
	std::string buffer = "location /{index index.php;}";
	Location location(buffer);

	EXPECT_EQ(location.getIndexes(), location_index);
}

TEST(Location, setLocationIndexByMethod) {
	Location location;
	location.setIndexes(location_index);

	EXPECT_EQ(location.getIndexes(), location_index);
}

TEST(Location, addLocationIndexByMethod) {
	Location location;
	location.addIndex("index.php");

	EXPECT_EQ(location.getIndexes(), location_index);
}

std::set<std::string> location_indexes = {"index.html", "index.htm"};

TEST(Location, setLocationIndexesByBuffer) {
	std::string buffer = "location /{index index.htm index.html;}";
	Location location(buffer);

	EXPECT_EQ(location.getIndexes(), location_indexes);
}

TEST(Location, setLocationIndexesByMethod) {
	Location location;
	location.setIndexes(location_indexes);

	EXPECT_EQ(location.getIndexes(), location_indexes);
}

TEST(Location, setLocationAutoIndexOnByBuffer) {
	std::string buffer = "location /{autoindex on;}";
	Location location(buffer);

	EXPECT_EQ(location.getAutoIndex(), true);
}

TEST(Location, setLocationAutoIndexOnByMethod) {
	Location location;
	location.setAutoIndex(parser::AUTOINDEX_ON);

	EXPECT_EQ(location.getAutoIndex(), true);
}

std::string location_fastcgi = "/var/bin/php-cgi";

TEST(Location, setLocationFastCgiByBuffer) {
	std::string buffer = "location /{fastcgi_pass " + location_fastcgi + ";}";
	Location location(buffer);

	EXPECT_EQ(location.getFastCgi(), location_fastcgi);
}

TEST(Location, setLocationFastCgiByMethod) {
	Location location;
	location.setFastCgi(location_fastcgi);

	EXPECT_EQ(location.getFastCgi(), location_fastcgi);
}

std::set<std::string> location_empty_methods = {};

TEST(Location, setLocationEmptyLimitExceptByBuffer) {
	std::string buffer = "location /{limit_except {}}";
	Location location(buffer);

	EXPECT_EQ(location.getMethods(), location_empty_methods);
}

std::set<std::string> location_method_get = {"GET"};

TEST(Location, setLocationLimitExceptByBuffer) {
	std::string buffer = "location /{limit_except GET {}}";
	Location location(buffer);

	EXPECT_EQ(location.getMethods(), location_method_get);
}

TEST(Location, setLocationLimitExceptByMethod) {
	Location location;
	location.addMethod("GET");

	EXPECT_EQ(location.getMethods(), location_method_get);
}

TEST(Location, setLocationDenyLimitExceptByBuffer) {
	std::string buffer = "location /{limit_except GET {deny all;}}";
	Location location(buffer);

	EXPECT_EQ(location.getDenyMethods(), true);
}

TEST(Location, setLocationDenyLimitExceptByMethod) {
	Location location;
	location.setDenyMethods("all");

	EXPECT_EQ(location.getDenyMethods(), true);
}

std::string location_error_page_code = "404";
std::string location_error_page_path = "/404.html";
std::map<std::string, std::string> location_error_pages = {{location_error_page_code, location_error_page_path}};

TEST(Location, setLocationErrorPagesByBuffer) {
	std::string buffer = "location /{error_page " + location_error_page_code + " " + location_error_page_path + ";}";
	Location location(buffer);

	EXPECT_EQ(location.getErrorPageByCode(location_error_page_code), location_error_page_path);
	EXPECT_EQ(location.getErrorPages(), location_error_pages);
}

TEST(Location, addLocationErrorPagesByMethod) {
	Location location;
	location.addErrorPages(location_error_page_code + " " + location_error_page_path);

	EXPECT_EQ(location.getErrorPageByCode(location_error_page_code), location_error_page_path);
	EXPECT_EQ(location.getErrorPages(), location_error_pages);
}

TEST(Location, setLocationErrorPagesByMethod) {
	Location location;
	location.setErrorPages(location_error_pages);

	EXPECT_EQ(location.getErrorPageByCode(location_error_page_code), location_error_page_path);
	EXPECT_EQ(location.getErrorPages(), location_error_pages);
}

std::string location_return_code = "301";
std::string location_return_uri = "http://127.0.0.1:8080";
std::map<std::string, std::string> location_return = {{location_return_code, location_return_uri}};

TEST(Location, setLocationReturnByBuffer) {
	std::string buffer = "location /{return " + location_return_code + " " + location_return_uri + ";}";
	Location location(buffer);

	EXPECT_EQ(location.getReturnCode(), location_return_code);
	EXPECT_EQ(location.getReturnURI(), location_return_uri);
}

TEST(Location, setLocationReturnByMethod) {
	Location location;
	location.setReturn(location_return_code + " " + location_return_uri);

	EXPECT_EQ(location.getReturnCode(), location_return_code);
	EXPECT_EQ(location.getReturnURI(), location_return_uri);
}

