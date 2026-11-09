# 현재 상태 — 서버 명령 시계와 본 포즈를 분리 검증

| 항목 | 확인된 내용 |
|---|---|
| LOD1 실험 구간 | 같은 요청·기록된 script NativeSeconds에도 LeftHand MS 위치가 대상/대조 간 최대 0.410m 차이 |
| 전후 대조 | 같은 비교의 지정 전/이후 일부 구간은0m. 기존 단발 시험 로그 재분석이며 새 실행 아님 |
| 해석 | 콜백 수·명령 입력 시계 일치가 native pose 동기화를 보장하지 않음 |
| 미확인 | 내부 시계/블렌딩 원인, 클라이언트 포즈·실제 화면 영향, CPU 이득 |
| 적용률 정정 | 정상 이동/idle·1초 guard의 원본64 비용 문맥 추정6.61~7.43%. 초기 분석은 INVESTIGATE 누락 등으로 과소계산 |
| 자산 단서 | 표시5파트 source GeometryLOD에는 component01 선택만 존재. compiled 물리/복제 독립성까지 입증한 것은 아님 |
| 주기적 서버 LOD 정책 | 미구현·미채택. 클라이언트 역할별 pose 확인이 먼저 필요 |
| 전체 목표 | 64 기능·실제 시각·20ms p95 최종 수용 미완료 |

새 게임·PBO·원본 수정 없이 기존 증거를 분석했다. 이 보고서는 LOD1 실험의 결과이며 기존 정상 배포본을 수정하지 않았다.

[최신 상세 보고서](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/server-follower-pose-exposure-20261109.md) · [기존 단발 LOD 시험](https://github.com/colskigurt/dayz-npc-public-data/blob/main/reports/operations/server-follower-lod-one-shot-20261109.md) · [전체 보고서](https://github.com/colskigurt/dayz-npc-public-data/tree/main/reports/operations)
