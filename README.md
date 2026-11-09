# DayZ NPC Observatory — 현재 상태

2026-11-09: **상태 판정 재사용의 전체64 CPU 이득은 미확인.** 기존64 로그 완전표본374개에서 격자 후보156~1228개를 재구성했다. 합성시험의 후보0/2016 사이이며, 밀집 국소20.57% 감소를 전체CPU 절감으로 확장할 수 없다. 선택 로그에는 Crowd.UpdateTicks가 없다.

- [최신 상세 보고서](reports/operations/eligibility-existing-density-20261109.md)
- [구조화 결과](data/eligibility-existing-density-20261109.json)
- [직전 인자 분리: 밀집 이득·희박 악화](reports/operations/crowd-eligibility-factor-20261109.md)
- [클라이언트 전용 명령 경계](reports/operations/client-local-explicit-start-20261109.md)
- [기존 보고서](reports/operations/)

일반 적용 후보 미수용 유지. 조건부 캐시는 미구현, 새 게임·8→64 확대 없음. 원본·물리검사·등반 재시도 유지. 전체CPU/시각 개선 미측정,20ms p95는 목표로 남는다.
