package com.example.map3;

// 위치 데이터를 담는 클래스
public class LocationResponse {
    private double latitude;  // 위도
    private double longitude; // 경도

    // 위도를 반환하는 Getter
    public double getLatitude() {
        return latitude;
    }

    // 경도를 반환하는 Getter
    public double getLongitude() {
        return longitude;
    }

    // 위도를 설정하는 Setter
    public void setLatitude(double latitude) {
        this.latitude = latitude;
    }

    // 경도를 설정하는 Setter
    public void setLongitude(double longitude) {
        this.longitude = longitude;
    }
}
