# DayZ NPC / 대량 호드 현재 상태

문서 갱신:2026-11-04

자체 저폴리 P3D 로컬 프록시의1,024개별 형상과128객체×8형상 실행 프로브를 수행했다. 1,024개별 형상 배열은 화면에서 관찰했다. 8형상 묶음도 생성·source readback 구간을 완주했다. 개체당72삼각형/5본이며 AI·물리·전투·네트워크는 없다.

**최종 수용 아님.** 실제 Present 미측정,정확한 개체별 픽셀 가시성·이동 축/거리·팔다리 애니메이션 품질 미수용. 시작/끝 기준 구간 script dt p95도35→78ms로 변해 묶음 성능의 인과 비교가 불가능하다.20ms p95와1,000좀비 무프레임저하는 달성하지 않았다.

다음 단일 작업은 근접 카메라에서 root 변환과 실제 팔다리 연속 움직임 확인이다. 이후 동일 조건의 paired Present 비교를 진행한다. 별도 애니메이션 세션과 기존 NPC 기능/성능 결과는 이 시험과 구분한다.

[상세 보고서](reports/operations/npc-horde-render-paths-20261104.md) · [근거](evidence/npc-horde-render-paths-20261104.txt) · [JSON](api/public/npc-horde-render-paths.json)

[직전 통합 상태](reports/history/20261104-before-render-probe-README.md) · [AI 검토](AI_REVIEW.md) · [검증 정책](reports/operations/npc-validation-policy-20261103.md)

기존 상세 NPC 후보의 검증 이력은 직전 상태에 보존했다. 이번 프록시는 별도 자산 계열이며 기존 결과의 기능 복구·성능 개선으로 집계하지 않는다. 기존 서버/Workshop 배포는 변경하지 않았다. 시험 PBO 자동 서명·검증 성공,개인 키 배포0개.
