# DayZ NPC 연구 — 현재 상태 (2026-11-12)

**단일 호스트 통합의 사망 수명 선행 시험은 종료 접근 위반으로 미수용입니다.** 사망 후 포즈 평가가 지속됐지만 안전한 통합이나 최적화 성공이 아닙니다.

| 격리 현재 모델3개·12게임초·1회 | 결과 |
|---|---|
| 사망 렌더 역할 | Health0/IsAlive=false에서도 script 평가 지속 |
| 사후4.246~11.945초 | PreAnim/PrePhys154회·script Age7.7초 증가 |
| 살아있는 대조 vs 사망 렌더 Head MS | 측정 표본 최대차0m; 화면/전신 증거 아님 |
| 종료·명령 퇴역 전후 | **서버0xC0000005, 안전 수명 실패** |
| 정확한 네이티브 크래시 원인 | 미확정 |
| CPU·시각·전체 NPC·실제 부위 피해 | 미측정/미시험 |

기존 구현에서 수신체와 렌더를 alias하면 사망 시 표시 몸체도20m 아래로 이동·비활성화됩니다. 현재 역할을 바로 합치지 않습니다. 기존 게임 코드/PBO/자산은 보존했고 서명 검증된 프로브만 재사용했습니다. 크래시 전 관찰과 전체 미수용을 구분합니다.

[최신 보고서](reports/operations/single-host-death-lifecycle-capability-20261112.md) · [최신 작업](reports/operations-latest.md) · [선정 증거](data/single-host-death-lifecycle-20261112-evidence.txt) · [집계·실패 판정](data/single-host-death-lifecycle-20261112.json)

직전 조사: [동일Ray 재사용](reports/operations/same-solve-ray-shadow-eight-20261112.md)은206320주발Ray 중22기회(0.010663%)로 낮은 우선순위. [Native Move 벽 대체](reports/operations/native-root-world-safety-gate-20261112.md)는 벽 통과로 기각했습니다. 동일 조건의 유리한 재실행이나64 확대를 하지 않습니다.

현재 안전성이 입증된 큰 폭 CPU 절감 후보는 미확보입니다. 실패를 연쇄 기능 복구 개발로 바꾸지 않고 기존 정상 밀침·낙하·착지·FOLLOW·재등반과 사용자 시각 검증을 보존합니다. 전체64 기능·시각·성능 최종 수용과20ms p95 목표는 미완료입니다.

공개 JSON 파싱·핵심 필드·해시 검증과 전체 스키마 준수는 별개입니다. 기존 스키마 불일치는 남아 있습니다. [공개 검증 기록](verification/latest.json)
