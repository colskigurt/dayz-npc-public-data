# DayZ NPC Observatory — 현재 상태

2026-11-09: **군중 상태 판정만 재사용한 후보는 일반 적용 미수용.** 기존 전체 snapshot의 한 요소를 분리했다. 합성 512프레임 불일치0, 밀집 국소 비용 중앙 **20.57% 감소**, 희박 **5.60% 증가**였다. 한 프로세스의 배치별3쌍 결과이며 전체64 CPU 개선이 아니다. 사전 확대 기준 실패로8→64를 실행하지 않았다.

- [최신 상세 보고서](reports/operations/crowd-eligibility-factor-20261109.md)
- [구조화 결과](data/crowd-eligibility-factor-20261109.json)
- [직전 클라이언트 전용 명령 경계](reports/operations/client-local-explicit-start-20261109.md)
- [1mm 후보 비용 실패](reports/operations/foot-range-tolerance1-method-20261109.md)
- [기존 보고서](reports/operations/)

원본 게임 코드·PBO·등반 재시도·지지/충돌 검사는 유지했다. 새PBO/서명 대상 없음. 실제 전체CPU·Present·8/64 기능·시각 품질 개선은 이번에 측정하지 않았다. 20ms p95는 목표로 남는다.

밀집 전용 전환은 미구현이며 이번 실패 기준을 사후 변경하지 않았다. 다음 선택은 기존 실제64의 pair 밀도와 Crowd.Update 비용 근거에 달려 있다. 국소20%를 전체CPU20%로 환산하지 않는다.
