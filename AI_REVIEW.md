# 현재 상태 — 서버 정밀도 후보가 클라이언트 포즈 gate에서 실패

| 항목 | 확인된 내용 |
|---|---|
| 시험 | 고정 Run·AI/이동OFF·단일5파트, 서버 part1/3만 LOD1 |
| 변경 전 | 활성151표본, 포즈변화151회, 공통 본 최대차0m |
| 변경 후 | 처리 표지 활성3프레임 중3프레임 불일치, 최대0.261m |
| 대조 | 변경하지 않은 part0/2/4는0m 유지 |
| 판정 | 조기 중단·LOD0 복원·서버 detached·클라이언트 종료. 후보 미수용 |
| 미실행 | Walk/Idle, 지속 콜백비율, 복원 후 수렴,8/64 회귀,CPU/Present 비교 |
| 시각 품질 | 클라이언트 CPU 포즈를 측정했으며 실제 화면의 부드러움은 별도 미측정 |
| 생산 코드 | 기존 게임 코드·10개 PBO·물리/등반 동작 불변. 격리 진단 PBO만 서명/검증 |
| 최종 목표 | 64개체 CPU/Present 및20ms p95 목표 미달·미수용 |

파일 폴링 표지는 개입보다 늦다. 경계의 phase0-labelled5프레임을 원래 baseline 실패나 정확한 네트워크 지연으로 해석하지 않는다. 단일 실행이며 반복 검증 완료가 아니다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/client-role-pose-gate-failed-20261109.md) · [이전 서버 포즈/적용률 분석](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/server-follower-pose-exposure-20261109.md) · [전체 보고서](https://github.com/colskigurt/dayz-npc-public-data/tree/main/reports/operations)

다음은 기존 실패 후보를 재시험하기보다 클라이언트 포즈 의존성을 건드리지 않는 미검증 물리 질의 비용 후보를 기존 자료로 선별한다. 정상 밀침·낙하·착지·추적 재개·재등반은 보존한다.

기능 성공·실제 시각·성능을 독립 판정한다. 처리군의 native 내부 시간/복제 전달 경로는 미확정이다. 비용 개선을 주장하지 않는다. 기존 근거와 실패 보고서는 보존한다.
