class PrimeNumberGenerator{
 public:
     explicit PrimeNumberGenerator(int iStart) : iCurrent(iStart) {}
     int GetNextPrime() {
         bool bGenerated = false;
         while (!bGenerated) {
             bGenerated = true;
			 if (iCurrent == 1) {
				 iCurrent++;
				 continue;
             }
             for (int n = 2; n <= sqrt(iCurrent); n++) {
                 if (iCurrent % n == 0) {
                     iCurrent++;
                     bGenerated = false;
                     break;
                 }
             }
         }
         return iCurrent++;
     }
 private:
     int iCurrent;
};
