# DayZ NPC Observatory — 현재 상태

2026-11-09: **기존 국소 최적화 후보 재선정 완료, 새 구현 없음.** 현재 조사 범위에서 큰 전체CPU 절감이 입증된 즉시 적용 후보는 없다. 서버 보조 표시파트의 클라이언트 전용 분리는384→128서버 호스트 구성이 가능한지 검토할 구조적 후보이나, 그래프 시계·전환 위상 동기화 제어점이 아직 확보되지 않았다. 개수 감소를 CPU 절감률로 환산하지 않는다.

- [후보 선정표·동기화 접근 계약·위험](reports/operations/structural-candidate-boundary-review-20261109.md)
- [구조화 상태](data/structural-candidate-boundary-review-20261109.json)
- [접근 감속 보류 근거](reports/operations/approach-slowdown-readonly-review-20261109.md)
- [64체 CPU 비교 조건 불일치](reports/operations/selective-peer-block-cost64-screen-20261109.md)
- [기존 보고서 목록](reports/operations/)

기존 모델·정상 밀침·낙하·착지·추적·재등반 및 해결된 애니메이션 경로를 보존한다. 로컬 파트 분리는 구현·컴파일·게임 시험 미실행이다.20ms p95 목표 미달성, 기능·시각·성능은 별도 판정한다.

새 분리 후보를 적용 완료로 취급하거나 기존 서버LOD 실패를 해결한 것으로 해석하지 않는다. 공개 선언/소스에서 필요한 제어점을 확보하지 못한 것이지 전체 엔진에서 불가능함을 증명한 것은 아니다.
