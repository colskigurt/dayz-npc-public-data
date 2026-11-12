# DayZ NPC 연구 — 현재 상태 (2026-11-12)

**현재 단일 파트 커스텀 모델에서 동물 네이티브 Move 연결을 확인했습니다.** 평지1개체의 능력 진단이며 CPU 최적화 성공이 아닙니다.

| 확인 항목 | 결과 |
|---|---|
| 네이티브 Move hook / 기존 그래프 RequestedState | 실행·입력 전달 확인 |
|20게임초 PreAnim / PrePhys 콜백 |404 /404 |
| 걷기/달리기 요청 구간 순이동 |11.58m /27.64m |
| 전체 NPC 추적·전투·등반·낙하 전환 | 미검증 |
|CPU 절감·시각 부드러움·64개체 수용 | 미측정/미완료 |

입력1/2는m/s가 아닙니다. heading0에서−Z이동과idle의작은root이동이관찰돼 보정이필요합니다. 이프로브에는기존GroundBody·피해수신체가연결되지않았습니다. 원본13PBO/14클립/기능보존,프로브운영미채택입니다.

[최신 상세 보고서](reports/operations/native-animal-move-one-capability-20261112.md) · [선정 소스·40행 증거](data/native-animal-move-one-20261112-evidence.txt) · [최신 작업](reports/operations-latest.md) · [공개 상태](api/public/status.json)

이전 [8개체 솔버 귀속](reports/operations/solver-caller-ownership-eight-20261112.md):지상34.5%/밀침29.0%는호출비중이며전체CPU비중아님. [관절Sweep 감축8](reports/operations/joint-only-eight-diagnostic-20261112.md)은성능미수용입니다.

다음은native지상↔script등반/낙하의권한전환계약검토입니다. 기능을보존할수있는후보만소수개체후8→64로검증합니다.20ms Present p95는목표입니다.

공개 JSON의기존스키마불일치는별도미해결입니다. HTTP/파일검증과전체스키마통과를구분합니다. [검증 기록](verification/latest.json).

외부 검토:초기컴파일인자명오류2건과revision3의준비이벤트미연결·NPC생성0을별도보존했습니다. 성공실행은revision4단독1회이며앞선실패를반복성능시험에합산하지않습니다. 네이티브내부검사가줄었거나더싸다는결론은아직없습니다.
