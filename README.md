# DayZ NPC 연구 — 현재 상태 (2026-11-12)

**기존 DGNativeCommand의 root 적용 분기에서도 실제 이동을 확인했습니다.** 현재 모델 1개체의 능력 진단이며, 전체 NPC나 성능 개선 수용은 아닙니다.

| 최근 30게임초 진단 | 결과 |
|---|---|
| Native Move ↔ script 명령 교체 | 4회 관찰 |
| Script root 적용 누적 | 34.8568m, 실제 위치 이동도 관찰 |
| Script PreAnim / PrePhys | 198 / 198콜백 |
| Script Age | 두 번째 명령에서 4.95→0.05초 재시작 |
| CPU·월드 충돌·지지·화면 부드러움 | 미측정 / 미검증 |

시험 개체에는 기존 GroundBody·피해수신체·추적 AI가 연결되지 않았습니다. 실제 NPC에서는 GroundBody 분기가 root를0으로 설정하므로 플래그만 켜서 적용할 수 없습니다. 명령 자체 Age를 native graph 재생 시간으로 해석하지 않습니다. UUID 응답0도 재생 연속성 증거가 아닙니다.

[최신 보고서](reports/operations/native-script-root-handoff-capability-20261112.md) · [선정 코드·전환 로그](data/native-script-root-handoff-20261112-evidence.txt) · [최신 작업](reports/operations-latest.md) · [공개 상태](api/public/status.json)

이전 [현재 모델 Native Move 연결](reports/operations/native-animal-move-one-capability-20261112.md), [8개체 솔버 호출 귀속](reports/operations/solver-caller-ownership-eight-20261112.md), [관절 감축 미수용](reports/operations/joint-only-eight-diagnostic-20261112.md)을 보존합니다.

다음: 기존 script-root 경로의 벽·발판 가장자리 반응을 작은 조건에서 확인합니다. 기능을 보존할 후보만 이후8→64로 검증합니다.20ms Present p95는 목표입니다. 원본13PBO·14클립·정상 기능 보존, 프로브 운영 미채택입니다.

공개 JSON의 기존 스키마 불일치는 별도 미해결입니다. HTTP/해시 검증과 전체 스키마 통과를 구분합니다. [검증 기록](verification/latest.json).
