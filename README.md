# DayZ NPC 연구 — 현재 상태 (2026-11-12)

**현재 단일 렌더+피해 수신체 64개체의 서버 CPU 스택을 새로 수집했습니다. 비용 부분 귀속이며 최적화 성공은 아닙니다.**

| 현행 구성·독립 진단1회 | 관측 |
|---|---:|
| 논리 / 표시 / 피해 수신체 | 64 / 64 / 64 |
| 요청 / 분석 이벤트 구간 | 8초 / 10.786316초 |
| Ray/Sphere 경계 union | 대상 가중 표본17.84% |
| 선정 VM 범위 leaf / inclusive | 27.47% / 57.45% |
| 스택 누락 | **26.13%** |
| Lost Events / Buffers | 0 / 0 |
| 관측 생존·목표 획득·2m 이동 | 64/64 |
| 전체 gameplay 미션dt p50/p95/p99 | 22.0/32.6/38.0ms — Present 아님 |

Inclusive 항목은 겹치므로 합산하지 않습니다. 누락 스택은0비용이 아닙니다. 과거5/8파트의 비율과 현재 값을 성능 전후 비교로 쓰지 않습니다. 진단 뒤 약62게임초에 중단했으며 최종 라벨은2유효·56미완료·2낙하·4미해결입니다. 전체64회귀 통과가 아닙니다.

[최신 보고서](reports/operations/current-single64-cpu-attribution-20261112.md) · [최신 작업](reports/operations-latest.md) · [선정 증거](data/current-single64-cpu-attribution-20261112-evidence.txt) · [집계](data/current-single64-cpu-attribution-20261112.json) · [다음 후보 사전 검토](reports/operations/lazy-crowd-eligibility-review-20261112.md)

다음은 기존 eager 상태 캐시의 희박 악화를 피하도록 실제 쌍에서 처음 필요할 때만 판정하는 lazy 방식의 **소스 계약과 국소 선별**입니다. 아직 구현하지 않았고 전체 CPU 효과도 미입증입니다. 정상 밀침·낙하·착지·FOLLOW·재등반, 기존 쌍/힘 순서, 물리 질의와 애니메이션 갱신은 보존해야 합니다.

기존 방향 setter 생략·eager cache·Scratch·SupportSweepFast·Native Move 직접 교체의 미수용 기록은 유지합니다. 새 PBO/운영 코드 변경은 없고 시험·관리자 도우미는 종료됐습니다. 20ms p95와 기능·시각·성능 최종 수용은 미완료입니다.

계측 영수증 참고: legacy runner의 captureRequested=false는 비활성화한 기존 OS CSV 샘플러 필드입니다. 이번 WPR은 별도 요청/관리자 종료/ETL 영수증에서 시작·종료0으로 검증했습니다.

공개 JSON 파싱·핵심 필드·해시 검증과 전체 스키마 준수는 별개입니다. 기존 스키마 불일치는 남아 있습니다. [공개 검증 기록](verification/latest.json)
