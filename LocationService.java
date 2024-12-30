package com.example.map3;

import retrofit2.Call;
import retrofit2.http.GET;

// 서버와 통신할 인터페이스 정의
public interface LocationService {
    
    /**
     * GPS 데이터를 가져오는 GET 요청 메서드
     * 서버 경로에 맞춰 엔드포인트만 지정
     * @return LocationResponse 데이터를 반환하는 Call 객체
     */
    @GET("GPS.php")
    Call<LocationResponse> getLocation();
}