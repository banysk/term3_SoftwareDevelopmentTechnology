#pragma once
#include <string>
#include <utility>

class Date {
 public:
     Date(int year, int month, int day) :
         iYear(year), iMonth(month), iDay(day) {}
     bool IsLeap() {
         if (iYear % 400 == 0) return true;
         if (iYear % 4 == 0 && iYear % 100 != 0) return true;
         return false;
     }
     std::string ToString() {
         std::string str;
         if (iDay < 10) {
             str += '0';
             str += std::to_string(iDay) + '.';
         } else {
             str += std::to_string(iDay) + '.';
         }
         if (iMonth < 10) {
             str += '0';
             str += std::to_string(iMonth) + '.';
         } else {
             str += std::to_string(iMonth) + '.';
         }
         int iHelper = iYear;
         str += std::to_string(iHelper / 1000);
         iHelper %= 1000;
         str += std::to_string(iHelper / 100);
         iHelper %= 100;
         str += std::to_string(iHelper / 10);
         iHelper %= 10;
         str += std::to_string(iHelper);
         return str;
     }
     Date DaysLater(int days) {
         int DaysIn[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
         int SaveYear = iYear, SaveMonth = iMonth, SaveDay = iDay;
         iDay += days;
         while (iDay > DaysIn[iMonth - 1] + (this->IsLeap() && iMonth == 2)) {
             iDay -= DaysIn[iMonth - 1] + (this->IsLeap() && iMonth == 2);
             if (++iMonth == 13) {
                 iYear++;
                 iMonth = 1;
             }
         }
         std::swap(SaveYear, iYear);
         std::swap(SaveMonth, iMonth);
         std::swap(SaveDay, iDay);
         return Date{ SaveYear, SaveMonth, SaveDay };
     }
     int DaysLeft(const Date& date) const {
         int DaysIn[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
         int iResult = 0;
         for (int year = iYear; year < date.iYear; year++) {
             if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) {
                 iResult += 366;
             } else {
                 iResult += 365;
             }
         }
         for (int month = 1; month < iMonth; month++) {
             iResult -= DaysIn[month - 1];
             if (month == 2 && ((iYear % 400 == 0) ||
                 (iYear % 4 == 0 && iYear % 100 != 0))) {
                 iResult--;
             }
         }
         iResult -= iDay;
         for (int month = 1; month < date.iMonth; month++) {
             iResult += DaysIn[month - 1];
             if (month == 2 && ((date.iYear % 400 == 0) ||
                 (date.iYear % 4 == 0 && date.iYear % 100 != 0))) {
                 iResult++;
             }
         }
         iResult += date.iDay;
         return iResult;
     }

 private:
     int iYear;
     int iMonth;
     int iDay;
};
