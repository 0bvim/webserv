#include <gtest/gtest.h>
#include "Common.hpp"
#include <fstream>
#include <cstdio>
#include "Mime.hpp"
#include "standard.hpp"
#include <stdexcept>
#include <string>

class MimeTest : public ::testing::Test
{
protected:
	std::string tmpFile;
	Mime *mime;

	virtual void SetUp()
	{
		tmpFile = "test_mime.json";
		std::ofstream ofs(tmpFile.c_str());
		ofs << "{\"text/plain\":[\"txt\"],\"text/html\":[\"html\"],"
			<< "\"application/javascript\":[\"js\"],\"text/css\":[\"css\"],"
			<< "\"application/json\":[\"json\"],\"image/svg+xml\":[\"svg\"],"
			<< "\"application/xml\":[\"xml\"]}";
		ofs.close();
		try
		{
			mime = Mime::getInstance();
			mime->configure(tmpFile);
		}
		catch (const std::exception &e)
		{
			FAIL() << "Failed to create Mime object: " << e.what();
		}
	}

	virtual void TearDown()
	{
		mime = NULL;
		std::remove(tmpFile.c_str());
	}
};

TEST_F(MimeTest, GetTypeTxtReturnsTextPlain)
{
	std::string result = mime->getType("document.txt");
	EXPECT_EQ("text/plain", result);
}

TEST_F(MimeTest, GetTypeHtmlReturnsTextHtml)
{
	std::string result = mime->getType("index.html");
	EXPECT_EQ("text/html", result);
}

TEST_F(MimeTest, GetTypeNoExtensionReturnsDefault)
{
	std::string result = mime->getType("filename");
	EXPECT_EQ(standard::MIME, result);
}

TEST_F(MimeTest, GetTypeUnknownExtensionReturnsDefault)
{
	std::string result = mime->getType("image.jpeg");
	EXPECT_EQ(standard::MIME, result);
}

TEST_F(MimeTest, GetTypeJsReturnsApplicationJavascript)
{
	std::string result = mime->getType("script.js");
	EXPECT_EQ("application/javascript", result);
}

TEST_F(MimeTest, GetTypeCssReturnsTextCss)
{
	std::string result = mime->getType("style.css");
	EXPECT_EQ("text/css", result);
}

TEST_F(MimeTest, GetTypeJsonReturnsApplicationJson)
{
	std::string result = mime->getType("data.json");
	EXPECT_EQ("application/json", result);
}

TEST_F(MimeTest, GetTypeSvgReturnsImageSvgXml)
{
	std::string result = mime->getType("image.svg");
	EXPECT_EQ("image/svg+xml", result);
}

TEST_F(MimeTest, GetTypeXmlReturnsApplicationXml)
{
	std::string result = mime->getType("config.xml");
	EXPECT_EQ("application/xml", result);
}

TEST(MimeConstructorTest, InvalidFileThrowsRuntimeError)
{
	try
	{
		Mime::getInstance()->configure("nonexistent.json");
		FAIL() << "Expected std::runtime_error due to invalid file path";
	}
	catch (const std::runtime_error &e)
	{
		// Test passes
	}
	catch (...)
	{
		FAIL() << "Expected std::runtime_error";
	}
}
