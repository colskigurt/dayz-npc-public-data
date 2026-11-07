# Terrain single-pass64 CPU 선별: 미수용

## 1. 작업 목표
국소 메서드 개선이 전체64개체 서버 CPU 감소로 연결되는지 단일 OFF/ON 비교. 기존8개체 입구 정체는 별도 결함이며 full mobility 수용은 미완료.

## 2. 실제 수행
동일12 PBO·동일 혼합 씬, 활성화 플래그만 변경. 기존 Steam 실행 경로와 CPU 수집기 재사용. 각30벽시계초 수집. 양쪽 서버 retirement/RequestExit 후 종료, 클라이언트 정상 종료. 강제 종료·운영 배포 변경 없음.

## 3. 측정 결과
|항목|OFF|ON|
|---|---:|---:|
|서버 CPU ms/벽시계초|847.415|843.315|
|게임/벽시계|0.99866|1.00012|
|native/벽시계|1.01843|0.98178|
|미션 갱신/게임초|38.019|37.266|
|미션 dt p50/p95/p99 ms|26/46/55.62|27/47/55.66|
|끝점 시계 읽기 ms|110.339/28.176|87.274/28.698|
|인식·2m 이동|64/64|64/64|
|관찰 host 생존|320렌더+64receiver|320렌더+64receiver|
|구간 foot-ray counter delta|735220|338470|
|구간 joint-sweep counter delta|293301|295618|

전체 CPU 변화 -0.484%로 사전5% 선별 기준 미달. OFF 시계 읽기는100ms 제한 초과. ON certificate40400·terrain sample404000. 경로·작업량·관찰 구간이 다르므로 호출 감소를 순수 비용 절감으로 계산하지 않는다. 미션 dt는 클라이언트 Present가 아니다. Present/GPU/실제 표시 품질 미측정.

## 4. 원인 분석
확인: 후보 활성화 및 foot ray 감소, 전체 CPU 차이는 작음.
가설: 추가 certificate/SurfaceY 또는 다른 접촉·호스트 비용이 효과를 상쇄할 수 있음. 개별 self-time은 미확정.
국소 약60% 개선을 전체 CPU 개선으로 확장할 수 없음.
수집기는 끝점 두 지점만 시계를 읽도록 설계됨. 중간 빈 값은 실패가 아니며 초기 해석은 정정됨.

## 5. 검증 수준
진단 성공, 성능 선별 미수용. 단일쌍이며 반복 검증·전체 기능·시각·최종 수용 아님. 추가 비용 반복과 채택 보류.20ms 달성 주장 없음.

## 6. 변경 사항
격리 미션에 읽기 전용 usage 관찰 추가. 기존 배포 PBO 불변. 후보 PBO는 앞선 서명·검증 성공본 그대로 사용, 이번 재빌드 없음.
mission SHA256 a02427e84199ab797a8389dfe188681abc133366ad7845f0b638eb42a7019eff
candidate PBO SHA256 4c78c9916e4e1fe300e3e984d70ade8d56f82ef803f9998a340a7277597acd82

## 7. 원시 증거
실행 ID ANIM-terrain-single-pass-cost64-screen1-off/on.
내부 보존: server-cpu.csv, server-frame.tsv, part-command-clock.tsv, mixed-hosts.tsv, gameplay-eight.tsv, terrain-single-pass-use.tsv, 종료 기록, 비교 분석 및 해시 manifest. 민감 원시 파일은 공개하지 않음.

## 8. 잔여 문제
시작 위상42.0291/43.7166게임초 차이, OFF 시계 제한 위반, 단일쌍. 이 결과로 개선이나 동등성을 확정할 수 없음.
다음: 기존 전체 비용 귀속과 후보 추가 질의 부담을 재검토해 다른 구조 후보를 선정. 같은 backoff 튜닝·반복 시험 또는 기능 복구 연쇄로 확대하지 않음.
