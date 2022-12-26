#include <ClassReader.h>
#include <ClassParser.h>
#include <ConstantPool.h>
#include "gtest/gtest.h"
#include <glog/logging.h>
#include <memory>
#include <string>

using namespace JVM;
namespace unit_test {
class ClassParserTest : public testing::Test {
  protected:
  std::shared_ptr<ClassData> data;
  std::shared_ptr<ClassFile> classFile;
  static int pos;
  ClassParserTest() {
  }

  ~ClassParserTest() override {
  }

  void SetUp() override {
    std::string classDir = "/home/android/jvm-demo/tests/javasample";
    DirClassReader reader(classDir);
    std::string className = "com.sample.Sample";
    std::string classPath = classNameToClassPath(className);
    data = reader.readClass(classPath);
    ASSERT_EQ(data->readErrno, SUCCEED);
    classFile = std::make_shared<ClassFile>();
    //ASSERT_TRUE(checkClassMagic(data->data));
  }
  

  void TearDown() override {
  }
};

int ClassParserTest::pos = 0;

TEST_F(ClassParserTest, parseAndCheckMagic) {
  //int pos = 0;
  JVM::parseAndCheckMagic(data, classFile, pos);
  ASSERT_EQ(classFile->magic, 0xCAFEBABE);
}

TEST_F(ClassParserTest, parseAndCheckVersion) {
  //int pos = sizeof(classFile->magic);
  LOG(INFO) << "parseAndCheckVersion current pos = " << pos;
  JVM::parseAndCheckVersion(data, classFile, pos);
  //ASSERT_EQ(pos, sizeof(classFile->magic) + sizeof(classFile->minorVersion) + sizeof(classFile->majorVersion));
  ASSERT_EQ(classFile->minorVersion, 0x0);
  ASSERT_EQ(classFile->majorVersion, 0x34);
}

TEST_F(ClassParserTest, parseConstantPool) {
  //int pos = 8;
  LOG(INFO) << "parseConstantPool current pos = " << pos;
  JVM::parseConstantPool(data, classFile, pos);
  ASSERT_NE(classFile->constantPool->constantPoolCount, 0);
  ASSERT_EQ(classFile->constantPool->constantPoolCount, 0xA0);
}
TEST_F(ClassParserTest, parseAccessFlags) {
  LOG(INFO) << "parseAccessFlags current pos = " << pos;
  JVM::parseAccessFlags(data, classFile, pos);
  ASSERT_EQ(classFile->accessFlags, 0x21);
}
TEST_F(ClassParserTest, parseThisClass) {
  LOG(INFO) << "parseThisClass current pos = " << pos;
  JVM::parseThisClass(data, classFile, pos);
  ASSERT_EQ(classFile->thisClass, 0x22);
}

TEST_F(ClassParserTest, parseSuperClass) {
  LOG(INFO) << "parseSuperClass current pos = " << pos;
  JVM::parseSuperClass(data, classFile, pos);
  ASSERT_EQ(classFile->superClass, 0x23);
}

TEST_F(ClassParserTest, parseInterfaces) {
  LOG(INFO) << "parseInterfaces current pos = " << pos;
  JVM::parseInterfaces(data, classFile, pos);
  ASSERT_EQ(classFile->interfaces.size(), 0);
}

TEST_F(ClassParserTest, parseFieldInfos) {
  LOG(INFO) << "parseFieldInfos current pos = " << pos << " superClass = ";
  int mockPos = 8;
  JVM::parseConstantPool(data, classFile, mockPos);
  ASSERT_EQ(classFile->constantPool->constantPoolCount, 0xA0);
  JVM::parseFieldInfos(data, classFile, pos);
  ASSERT_EQ(classFile->fields.size(), 0x0C);
}
TEST_F(ClassParserTest, parseMethodInfos) {
  LOG(INFO) << "parseMethodInfos current pos = " << pos << " superClass = ";
  int mockPos = 8;
  JVM::parseConstantPool(data, classFile, mockPos);
  ASSERT_EQ(classFile->constantPool->constantPoolCount, 0xA0);
  JVM::parseMethodInfos(data, classFile, pos);
  ASSERT_EQ(classFile->methods.size(), 0x03);
}
TEST_F(ClassParserTest, parseAttributeInfos) {
  LOG(INFO) << "parseAttributeInfos current pos = " << pos << " superClass = ";
  int mockPos = 8;
  JVM::parseConstantPool(data, classFile, mockPos);
  ASSERT_EQ(classFile->constantPool->constantPoolCount, 0xA0);
  JVM::parseAttributeInfos(data, classFile, pos);
  ASSERT_EQ(classFile->attributes.size(), 0x03);
}

TEST_F(ClassParserTest, parse) {
  pos = 0;
  classFile = JVM::parse(data);
  ASSERT_EQ(classFile->magic, 0xCAFEBABE);
  ASSERT_EQ(classFile->minorVersion, 0x0);
  ASSERT_EQ(classFile->majorVersion, 0x34);
  ASSERT_EQ(classFile->constantPool->constantPoolCount, 0xA0);
  ASSERT_EQ(classFile->accessFlags, 0x21);
  ASSERT_EQ(classFile->thisClass, 0x22);
  ASSERT_EQ(classFile->superClass, 0x23);
  ASSERT_EQ(classFile->interfaces.size(), 0);
  ASSERT_EQ(classFile->fields.size(), 0x0C);
  ASSERT_EQ(classFile->methods.size(), 0x03);
  ASSERT_EQ(classFile->attributes.size(), 0x03);
}

}