# 현재 상태 — 비용 후보 중복 검토, 물리 마스크 이상 없음

| 항목 | 최신 판정 |
|---|---|
| 물리 질의 마스크 | 현행 서버 runtime Floors33064/Walls67408168, 의도한OR와일치·추가비트0 |
| 검증 범위 | headless·NPC0·클라이언트0·setter0. 비용 개선 시험 아님 |
| 애니메이션 후보 | 직전 서버 follower LOD 변경 후 클라이언트 본 최대0.261m차이, 즉시 원복·기각 |
| 중복 제외 | SupportFast·Scratch·스냅샷·negative-backoff8/16·Solve뒤0변위는 이미 연구됨 |
| 성능 | 새 절감 없음. 광선58%감소했어도 과거SupportFast 첫 쌍 CPU+5.19%/Present+7.12% |
| 한계 | 위SupportFast는 수신기 포함384호스트·완전비교1쌍, 작업량/발판비율 차이 존재. 순수함수비용 아님 |
| 최종 수용 | 64기능·실제 시각·전체성능 목표 미완료.20ms p95는목표 |

이번 제한된 검토에서 바로 구현할 새 고가치 안전 후보를 확보하지 못했다. 이것을 엔진 전체의 최적화 불가능으로 확대하지 않는다. 오래된 DESIGN 상태보다 실제 후속 family-disposition을 우선한다. 기존 게임 코드/PBO/정상 밀침·낙하·착지·추적·재등반은 보존했다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/physics-mask-and-candidate-review-20261109.md) · [직전 클라이언트 포즈 gate 실패](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/client-role-pose-gate-failed-20261109.md) · [보고서 전체](https://github.com/colskigurt/dayz-npc-public-data/tree/main/reports/operations)

다음 산출물은 기존64 기록의 실제 비용 구간과 상태/작업량/프로세스CPU를 어디까지 동시에 연결할 수 있는지 밝히는 구간표다. 기존 표가 있으면 재생성하지 않으며, 호출량이나inclusive표본을self-time으로 바꾸지 않는다.
