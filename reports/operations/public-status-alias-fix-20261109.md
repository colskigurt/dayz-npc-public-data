# 공개 상태 별칭 일관성 복구

## 1. 작업 목표
외부 세션이 읽는 기존 공개 상태 경로의 최신 판정을 일치시킨다. NPC 연구 범위 선택 대기는 유지하며 게임 코드는 변경하지 않는다.

## 2. 실제 수행
인증 없는 실제 Raw HTTP GET으로 api/public/status.json, api/public/current-status.json, current-status.json, reports/latest.md를 확인했다. status.json과 루트 current-status는 structural-candidate-boundary-review를, 기존 api/public/current-status는 server-follower-pose-exposure-review를 가리켰다. 최신 게시 과정에서 기존 별칭 경로를 누락한 기록 오류다.

## 3. 측정 결과
네 경로 모두 HTTP200이었다. status/root current는1151바이트, api/public/current-status는6186바이트로 sourceRunId와 갱신시각이 달랐다. 이것은 DNS/접근 오류가 아니라 공개 데이터 갱신 누락이다. 새 게임·CPU/Present·시각 측정0이다. 게시 후 동일 commit의3개 상태 경로를 HTTP/본문SHA256/JSON으로 검증한다. 후속 검증 영수증 이전에는 외부 복구 완료로 판정하지 않는다.

## 4. 원인 분석
현재 발행 묶음은 api/public/status.json과 루트 current-status.json을 갱신했으나 기존 api/public/current-status.json을 포함하지 않았다. 구현 문제와 연구 결론을 바꾸지 않고 기존 별칭 응답을 최신 canonical 내용으로 맞춘다. 이전 응답 전체는 이력 파일에 보존한다. 이전 보고서/원시 실험을 덮어쓰지 않는다.

## 5. 검증 수준
불일치 원인 재현: 진단 성공. 게시/외부 동등성: 후속 영수증으로 별도 판정. 기능·시각·성능 개선 주장 없음. 연구 선택 답변은 아직 없으며 기준 변경/서버클라이언트 분리 구현은 미착수다.

## 6. 변경 사항
api/public/current-status.json 별칭을 기존 api/public/status.json의 동일 스냅샷으로 교체한다. 원본 generatedAt/sourceRunId를 유지하여 새 실험으로 오인하지 않도록 한다. 교체 전 응답은 reports/history/20261109-before-status-alias-fix.json에 저장한다. 이 감사 보고서만 추가한다. 게임 코드/PBO/운영 서버 변경0, 새 서명 대상0.

## 7. 원시 증거
교체 전 Git commit 0bf48613dce8e66b18ed3c264351445376ebf111. 상태 실행 ID ANIM-structural-candidate-boundary-review-20261109, 이전 별칭 ANIM-server-follower-pose-exposure-review1. 게시 묶음/HTTP 검증 영수증에 commit별 URL과 정확한 SHA256을 저장한다. PID·키·내부 원시로그를 새로 공개하지 않는다.

## 8. 잔여 문제
최신 게시 체크리스트에는 기존 별칭3개를 같은 원자적 Git tree에 포함한다. 실험 보고서의 내용 최신화와 접근 경로 일관성을 따로 검증한다. 현재64체 비용 개선과20ms p95 목표는 미완료이며 이 기록 복구를 최적화 성과로 세지 않는다.
