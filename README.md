# DayZ NPC Observatory — 현재 상태

2026-11-09: **상태 판정 캐시의 주력 확대를 보류.** 실제64 기준 한 실행에서 기존 군중Update 누적 범위는 경과시간의5.16%였다. 별도 서버CPU는807.22CPUms/벽시계초. 두 값은 서로 다른 측정이며 CPU self-time 비중/개선률이 아니다. 밀집 합성20.57% 이득을 전체64 개선으로 확장하지 않는다.

- [최신 상세 보고서](reports/operations/crowd-live-attribution-20261109.md)
- [구조화 결과](data/crowd-live-attribution-20261109.json)
- [기존64 밀도 대조](reports/operations/eligibility-existing-density-20261109.md)
- [인자 분리: 밀집 이득·희박 악화](reports/operations/crowd-eligibility-factor-20261109.md)
- [기존 보고서](reports/operations/)

64논리·320파트·64수신체 생존 인구 표본70회 확인.70초 진단 terminal은 유효10/미완료53/미해결1이며 전체 회귀 통과가 아니다. 후보 미적용,원본/물리검사/등반 재시도 유지. 새PBO 없음. Present·시각 개선 미측정.20ms p95 목표 미달성. 서버·클라이언트 종료 확인.
