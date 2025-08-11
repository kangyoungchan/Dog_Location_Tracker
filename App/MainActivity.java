package com.example.map3;

import android.os.Bundle;
import android.os.Handler;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class MainActivity extends AppCompatActivity implements OnMapReadyCallback {

    private GoogleMap googleMap;
    private LocationService locationService;
    private final Handler handler = new Handler();
    private Marker locationMarker;  // 위치 마커 참조 변수
    private static final int UPDATE_INTERVAL = 5000; // 위치 업데이트 간격 (5초)

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Retrofit 초기화
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl("http://121.170.42.202:9876/") // 실제 서버 URL로 교체
                .addConverterFactory(GsonConverterFactory.create())
                .build();
        locationService = retrofit.create(LocationService.class);

        // Google Maps 설정
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        if (mapFragment != null) {
            mapFragment.getMapAsync(this); // 지도 준비 완료 후 콜백 등록
        }
    }

    @Override
    public void onMapReady(@NonNull GoogleMap googleMap) {
        this.googleMap = googleMap;
        startLocationUpdates(); // 위치 업데이트 시작
    }

    // 위치 업데이트 시작 메서드
    private void startLocationUpdates() {
        handler.postDelayed(locationUpdater, 0); // 즉시 첫 위치 업데이트 수행
    }

    // 주기적으로 위치를 업데이트하는 Runnable
    private final Runnable locationUpdater = new Runnable() {
        @Override
        public void run() {
            fetchLocationData(); // 서버에서 위치 데이터 가져오기
            handler.postDelayed(this, UPDATE_INTERVAL); // 지정된 간격으로 반복 실행
        }
    };

    // 서버에서 위치 데이터를 받아와 지도에 표시하는 메서드
    private void fetchLocationData() {
        Call<LocationResponse> call = locationService.getLocation();
        call.enqueue(new Callback<LocationResponse>() {
            @Override
            public void onResponse(@NonNull Call<LocationResponse> call, @NonNull Response<LocationResponse> response) {
                if (response.isSuccessful() && response.body() != null) {
                    double latitude = response.body().getLatitude();
                    double longitude = response.body().getLongitude();
                    LatLng newLocation = new LatLng(latitude, longitude);

                    // 기존 마커가 있으면 위치만 업데이트, 없으면 새 마커 생성
                    if (locationMarker == null) {
                        locationMarker = googleMap.addMarker(
                                new MarkerOptions()
                                        .position(newLocation)
                                        .title("실시간 위치")
                        );
                        googleMap.moveCamera(CameraUpdateFactory.newLatLngZoom(newLocation, 15));
                    } else {
                        locationMarker.setPosition(newLocation);
                    }
                } else {
                    Toast.makeText(MainActivity.this, "서버 응답 실패", Toast.LENGTH_SHORT).show();
                }
            }

            @Override
            public void onFailure(@NonNull Call<LocationResponse> call, @NonNull Throwable t) {
                Toast.makeText(MainActivity.this, "서버 요청 실패: " + t.getMessage(), Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        handler.removeCallbacks(locationUpdater); // 액티비티 종료 시 위치 업데이트 중지
    }
}





















