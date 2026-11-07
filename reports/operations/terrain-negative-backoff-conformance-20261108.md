# 지형 인증 backoff 소스·순서 검증과 접촉점 오차 진단

## 1. 작업 목표

거절된 비지형 인증의8호출 backoff를 최소 시험 코드로 만들고, 원래 솔버 fallback과 재시도 순서를 검증한다.

## 2. 실제 수행

이전 지형 통합/루프분리 코드를 재사용한 격리 하위클래스를 생성했다. 현재453d6a9e PBO의 Ground member와 소스 일치·PBO 체크섬을 확인하고, 후보의 lift거부/관절Sweep/발접촉fallback 이하 tail이 원본과 정확히 같음을 검증했다. 서버 전용10호출 순서 시험1회, 같은 기준을 유지하고 실제 오차 열만 추가한 진단1회를 실행했다. 클라이언트/새PBO/영상/비용 측정 없음.

## 3. 측정 결과

두 실행 모두 첫 roof 비등록물체 Overlap 거절 후 Remaining=8→0, 이후9번째 인덱스에서 인증 재시도. 거절 호출과8회 bypass 모두 발ray10개/관절Sweep4개 유지. 재시도 지형 경로는 발ray0/관절Sweep4개. 모든10호출 반환값·이유는 원본과 일치하고 lift차이는0.1mm 이내였다. 엄격 접촉점 비교는 마지막 호출에서 실패했다. 진단의 실제 최대오차는0.0000305176m, 약0.0305mm다. 새 p50/p95/p99·CPU/Present 성능값은 없다.

## 4. 원인 분석

새 순서 시험이 재사용한 SamePoints 헬퍼는1µm였고 이전 지형 Contract는0.1mm를 사용했다. 새 시험 작성에서 이 차이를 놓쳤다. 실제오차는 이전 국소 허용치 이내이나 새 엄격 gate는 실패로 보존한다. 이전 국소 허용치를 인게임 허용 오차로 전용하거나 평균 시각 품질이 좋다고 주장하지 않는다. backoff제어는 확인됐지만 positive 지형대체 자체는 원본과 bit-identical하지 않다.

## 5. 검증 수준

소스 정합/Enforce컴파일/negative backoff순서 진단 성공. 새 엄격 접촉점 conformance 실패. 비용/8·64동적 기능/시각 품질/최종 수용 미검증. 두 서버는 종료 요청후exit−1, 정상 프로세스 종료 통과가 아니다. 실행 종료를 확인했다.

## 6. 변경 사항

NFTerrainNegativeBody.c, source-proof.json, 격리 순서 미션과 파생 요약 추가. 원본 PBO/배포 코드/자산/기존기록 변경 없음. 새 서명 대상 없음. 후보는 원래 솔버를 super로 호출하며 성공 지지/points/lift를 저장하지 않는다. 양성clear캐시·틱/레이어변경·Drive/Displace 병합 없음.

## 7. 원시 증거

ANIM-terrain-negative-backoff-conformance1 및 ANIM-terrain-negative-backoff-diagnostic2. 각10행 negative-backoff-sequence.tsv, 미션/고정계획/서버기록, source-proof 및 conformance-summary를 보존했다. 공개 자료는 집계·파일명/SHA256이며 내부 경로·PID를 제외했다.

## 8. 잔여 문제

다음 단일 산출물은 기존0.1mm 국소 계약을 명시하고 실제오차도 유지하는 original/sham/후보 비용 비교 하네스다. 첫 거절과8회 bypass/반복 재시도 비용을 따로 계수하고, 기존8조건에 비등록물체 및blocked→clear를 추가한다. frozen clock이 아닌 solve횟수로 backoff가 반복돼야 한다. 기존5%이득/제외경로3%상한을 유지하며 미달이면8/64로 확대하지 않는다. 현재 엄격실패를 통과로 덮어쓰지 않는다.
