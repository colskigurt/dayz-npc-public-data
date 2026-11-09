# DayZ NPC Observatory — 현재 상태

2026-11-09: **64→248 골격 목록 고정 쌍의 서버 공통64본 포즈가 일치했다.** 18.9483초/60,736행, 위상 일치 행 최대 오차0m. 클라이언트에서 두 부분 메시를2시점 관찰했지만 관측기 준비 신호와 클라이언트 포즈 로그는 없었다.

- [최신 상세 보고서](reports/operations/cardinality64-pair-20261109.md)
- [구조화 결과](data/cardinality64-pair-20261109.json)
- [직전 정적 동등·단독 생성](reports/operations/cardinality64-factory-20261109.md)
- [이전 기준 factory 실패 분리](reports/operations/cardinality-factory-crash-20261109.md)
- [기존 보고서](reports/operations/)

**부분 진단이며 미수용.** 의도10Hz와 실제 수집 간격이 달랐고 네트워크 쌍 퇴역도 확인되지 않았다. 같은 소유 서버를360초까지 추적한 뒤 강제 정리했다. 클라이언트는 정상 종료, 현재 소유 게임 프로세스 없음. 기존 이동·물리 코드는 보존했다.

다음은 기존 하네스의 준비·표본 제한·퇴역 흐름 확인이다. 전체 가중 본 통합·실제 화면 부드러움·8/64 기능·CPU 절감·20ms p95 목표는 미해결이며 이번 결과로 성공을 주장하지 않는다.
