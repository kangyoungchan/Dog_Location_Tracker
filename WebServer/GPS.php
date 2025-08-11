<?php
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST, GET');  // GET 허용
header('Access-Control-Allow-Headers: Content-Type');
header('Content-Type: application/json');
header('Cache-Control: no-cache, no-store, must-revalidate');
header('Expires: 0');

// 파일 경로
$file = 'coordinates.txt';

// GET과 POST 요청에 따라 다르게 처리
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // POST 요청일 때
    $latitude = isset($_POST['latitude']) ? $_POST['latitude'] : null;
    $longitude = isset($_POST['longitude']) ? $_POST['longitude'] : null;
} 
elseif ($_SERVER['REQUEST_METHOD'] === 'GET') {
    // GET 요청일 때
    $latitude = isset($_GET['latitude']) ? $_GET['latitude'] : null;
    $longitude = isset($_GET['longitude']) ? $_GET['longitude'] : null;
}

// 값 검증
$latitude = filter_var($latitude, FILTER_VALIDATE_FLOAT);
$longitude = filter_var($longitude, FILTER_VALIDATE_FLOAT);

// 유효한 좌표가 전달되었을 때만 파일에 저장
if ($latitude !== false && $longitude !== false) {
    // 파일에 저장
    $entry = date('Y-m-d H:i:s') . " - Latitude: $latitude, Longitude: $longitude\n";
    file_put_contents($file, $entry, FILE_APPEND);

    // JSON 형식으로 최근 위도와 경도 반환
    $response = [
        'latitude' => $latitude,
        'longitude' => $longitude
    ];
    echo json_encode($response);
    exit;
} else {
    // GET 요청 시 마지막 위치값을 읽어옴
    $lines = file($file, FILE_IGNORE_NEW_LINES);
    if (!empty($lines)) {
        $lastLine = end($lines);
        preg_match('/Latitude: ([0-9.-]+), Longitude: ([0-9.-]+)/', $lastLine, $matches);
        if (count($matches) === 3) {
            $latitude = floatval($matches[1]);
            $longitude = floatval($matches[2]);
            $response = [
                'latitude' => $latitude,
                'longitude' => $longitude
            ];
            echo json_encode($response);
        } else {
            // 파일이 비어있거나 유효한 좌표가 없을 때는 아무 것도 반환하지 않음
            echo json_encode(['error' => 'No valid coordinates found']);
        }
    } else {
        // 파일이 비어있을 때도 아무 것도 반환하지 않음
        echo json_encode(['error' => 'No valid coordinates found']);
    }
}
