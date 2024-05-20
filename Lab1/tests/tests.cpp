
#include <gtest/gtest.h>
#include "../src/employee.h"

TEST(EmployeeTest, DefaultValues) {
    employee emp;
    emp.num = 1;
    strcpy(emp.name, "John");
    emp.hours = 40.0;

    EXPECT_EQ(emp.num, 1);
    EXPECT_STREQ(emp.name, "John");
    EXPECT_DOUBLE_EQ(emp.hours, 40.0);
}

TEST(EmployeeTest, CheckNegativeHours) {
    employee emp;
    emp.num = 1;
    strcpy(emp.name, "John");
    emp.hours = -5.0;

    EXPECT_LT(emp.hours, 0.0);
}
