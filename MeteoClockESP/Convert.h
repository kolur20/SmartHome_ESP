#pragma once

class Convert {
public:
static IPAddress str2IP(String str);
static int GetIpBlock(int index, String str);


  
};

int Convert::GetIpBlock(int index, String str) {
    char separator = '.';
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = str.length()-1;
  
    for(int i=0; i<=maxIndex && found<=index; i++){
      if(str.charAt(i)==separator || i==maxIndex){
          found++;
          strIndex[0] = strIndex[1]+1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
    }
    
    return found>index ? str.substring(strIndex[0], strIndex[1]).toInt() : 0;
}


IPAddress Convert::str2IP(String str) {

    IPAddress ret( GetIpBlock(0,str),GetIpBlock(1,str),GetIpBlock(2,str),GetIpBlock(3,str) );
    return ret;

}
