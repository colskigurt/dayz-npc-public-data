# DayZ NPC — 현재 상태
갱신: 2026-11-08. 이번 작업은64개체30초전체CPU 단일OFF/ON 선별이다. 운영본은 유지한다.

현재: [복구 탐색 수정안64개체 CPU 선별](reports/operations/support-dominance-cost64-screen-20261108.md). **796.659→793.527 CPU ms/벽시계초(-0.393%). 사전5%기준 미달·미채택·같은 후보 반복 확대 없음.**

| 대상 | 현재 판단 |
|---|---|
| 주력 | 64개체 전체 CPU 비용 절감, 기존 밀침·낙하·재추적 보존 |
| 애니메이션 | 이전 사용자 시각 확인은 보존. 최신 비용 진단에서 새 시각 품질은 미측정 |
| 최근 8개체 후보 비교 | 양쪽 생존·인식·이동8/8. 전체 milestone7/8대5/8, 유효terminal6/8대4/8로 미수용 |
| 최근 64개체 비용 선별 | CPU796.659→793.527 ms/벽시계초,-0.393%,단일쌍·5% gate미달 |
| 고정 Idle 384호스트 진단 | 기본/본/접촉 포함 CPU 평균 224.272/231.156/282.727 ms/벽시계초. 1프로세스·조건별2구간 |
| 최신 위치·방향 분석 | 기존 CPU 가중 표본 144/8101(1.78%) 포함. 스택 누락28%, self-time 아님 |
| 최적화 채택·20ms 목표 | 최근 후보 미채택. 전체 p95 20ms 달성 미확인 |

[최신 상세 진단](reports/operations/transform-update-attribution-20261108.md) · [고정 Idle 진단](reports/operations/host-floor-pose-contact-isolation-20261108.md) · [64개체 후보 선별](reports/operations/pair-radius-cost64-screen-20261108.md) · [외부 AI 검토 안내](AI_REVIEW.md) · [공개 JSON](api/public/status.json)

다음: 기존 CPU/스크립트/호스트 격리 증거에서 남은 큰 실행 범위와 변경 가능한 API 경계를 대조한다. 이 후보를 운영본에 반영하지 않는다. 새로운 보간·업데이트 정책이나 기능 재설계를 시작하지 않는다. CPU ms/벽시계초는 프레임 시간이 아니다. 정지·이동 조건을 직접 차감하지 않는다.

공개 요약 동기화 누락을 이번에 정정했다. 과거 보고서와 [교체 전 진입점](reports/archive/b47311ff/)은 보존한다.
