/*
 * File:   GPS.c
 * Author: sooju
 *
 * Created on 2025? 8? 11? (?), ?? 10:44
 */


#include "GPS.h"

void parseGPSData(char* data, char* latitude, char* longitude) {
    char* ptr = strstr(data, "$GPRMC");
    if (ptr != NULL) {
        sscanf(ptr, "$GPRMC,%*f,A,%[^,],N,%[^,],E", latitude, longitude); 
    }
}


void convertToDecimalDegrees(const char* dms, double* decimalDegrees) {
    int degrees = atoi(dms) / 100;                    
    double minutes = atof(dms) - (degrees * 100);      
    *decimalDegrees = degrees + (minutes / 60.0);      
}


void updateCoordinates(char* data, double* latitudeDecimal, double* longitudeDecimal) {
    char latitudeDMS[15], longitudeDMS[15];

   
    parseGPSData(data, latitudeDMS, longitudeDMS);

    
    convertToDecimalDegrees(latitudeDMS, latitudeDecimal);
    convertToDecimalDegrees(longitudeDMS, longitudeDecimal);

  
    if (data[19] == 'S') {  
        *latitudeDecimal = -(*latitudeDecimal);
    }
    if (data[34] == 'W') {  
        *longitudeDecimal = -(*longitudeDecimal);
    }
}

bool isValidGPSData(const char* data) {
    
    if (strstr(data, "$GPRMC") != NULL) {
        
        if (strstr(data, ",A,") != NULL) {
            return true;  
        }
            
        return false;  
    }
   
    return false;  
}

