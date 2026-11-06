# DayZ NPC 현재 상태

- 최신 준비: [receiver-skeleton-candidate-20261107](reports/operations/receiver-skeleton-candidate-20261107.md)
- 별도 피해 수신기 골격 후보: 원본250노드→보호·부모 포함27노드. 14클립+골격 변환 및 서명 빌드 완료.
- **실제 포즈·피격·8/64 기능·CPU·시각은 아직 미검증.** 호스트 수·충돌 검사·클럭 변경 없음.
- Workbench 변환 후 종료 때 기존 script-leak assertion 재현. 정상 종료 통과 아님.
- 다음: 기존14클립 비교에서 전체/축소 수신기의 실제 본 변환·피격 헐 일치 검증.
- [공개 JSON](api/public/receiver-skeleton-candidate.json)
- [직전64비용 비교: 셀 풀 미수용](reports/operations/grid-cell-pool-cost-review-20261107.md)

기존 상세 기록: [README.md 보존본](reports/history/receiver-skeleton-candidate-20261107-before-README.md)
