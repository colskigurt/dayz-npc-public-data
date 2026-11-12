# DayZ NPC 연구 — 현재 상태 (2026-11-12)

**피해 수신체를 Native Move로 바꾸는 후보는 제자리 유지 실패로 미수용입니다.** 이동 입력0이어도 보행·달리기 포즈의 root 이동이 발생했습니다.

| 현재 수신체 모델2개·12게임초·1회 | 결과 |
|---|---:|
| 기존 script root0 수신체 이동 | 0m |
| Native Move/input0 보행·달리기 순이동 | 8.395686m / 18.961915m |
| 초기 정착점 대비 최대 수평거리 | 27.533278m |
| 대응 Head/Hips MS 최대차 | 각각0m (세계 위치 일치 아님) |
| 종료 | 서버0xC0000005, 안전 수명 실패 |
| CPU·화면·전체 NPC·실제 피격 | 미측정/미시험 |

기존2호스트는 그대로 보존했습니다. 위치 강제 보정이나 새 보간으로 실패를 숨기지 않습니다. 살아있는대상만의 시험도Finish에서크래시했으므로직전[사망수명시험](reports/operations/single-host-death-lifecycle-capability-20261112.md)의오류를사망에만귀속하지않습니다. 같은raw stack후보RVA0x220989는관찰이며정확한손상원인은미확정입니다.

[최신 보고서](reports/operations/receiver-native-move-stationarity-20261112.md) · [최신 작업](reports/operations-latest.md) · [선정 증거](data/receiver-native-move-stationarity-20261112-evidence.txt) · [집계](data/receiver-native-move-stationarity-20261112.json)

기각·보류 연구: Native Move 벽 직접 대체, 동일Ray재사용의낮은기회(0.010663%), 단일호스트의안전수명, 이번수신체명령직접교체. 반복으로유리한수치를찾거나64로확대하지않습니다. 기존단일렌더+수신체의정상밀침·낙하·착지·FOLLOW·재등반과사용자시각관찰을보존합니다.

현재안전하게입증된큰폭CPU절감후보는미확보입니다. 전체64기능·시각·성능수용과20ms p95목표는미완료입니다. 과거8/5파트와현재단일파트·서로다른활동조건을혼합하지않습니다.

공개JSON파싱·핵심필드·해시검증과전체스키마준수는별개입니다. 기존스키마불일치는남아있습니다. [공개 검증 기록](verification/latest.json)
