# DayZ NPC 연구 — 현재 상태 (2026-11-12)

| 최근 격리 비교: 8개체 | 기준 | 관절 Sweep만1/100 |
|---|---:|---:|
| 서버 CPU 평균, ms/벽초 | 165.74 | 164.39 |
| CPU p95, ms/벽초 | 218.40 | 250.14 |
| 주 관절 Sweep/솔버 | 3.986 | 0.0401 |
| 발 Ray/솔버 | 10 | 10 |
| 생존·인식 후2m이동 | 8/8 | 8/8 |
| 종료 유효 종결 | 6/8 | 6/8 |

**평균 CPU 개선은 관찰상0.815%에 그쳤고 꼬리는 악화돼 미수용입니다.** 동일미션·PBO, mode만0/4, 기준→후보 독립1쌍입니다.64확대·운영채택은 하지 않았습니다. 애니메이션 설정은 유지했고 실제 화면 품질·Present는 미측정입니다.

[최신 상세 보고서](reports/operations/joint-only-eight-diagnostic-20261112.md) · [최신 작업](reports/operations-latest.md) · [선정 증거](data/joint-only-eight-20261112-evidence.txt) · [공개 상태](api/public/status.json)

이전 결과: [통합1/100 CPU감소와15개체 지하낙하](reports/operations/rate100-200-comparison-20261112.md), [바닐라 초기 활성 작업량 불일치](reports/operations/vanilla-custom-workload-audit-20261112.md). 조건이 달라 이번8결과와 직접 합산하지 않습니다.

다음: 이미 시험한 후보를 반복하지 않고, 이동/밀침 솔버의 호출 소유권과 포즈 입력 준비의 실제 중복 여부를 기존 소스·증거로 확인합니다. 목표20ms p95와 기능·시각 최종 수용은 미완료입니다.
