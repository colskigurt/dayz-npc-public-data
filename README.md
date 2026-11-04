# DayZ NPC / 대량 호드 현재 상태

<!-- NPC_CPU_CURRENT_BEGIN -->
## 현재 CPU 최적화 상태

64개체 정지Run·실제SyncInterval0·500FPS 상한, 8→5/5→8/8→5의3쌍 모두5파트 CPU Busy/Present p95 감소. 쌍별 Present 변화 중앙값 -16.69%, CPU Busy -16.81%, 절대 Present p95 차이0.99~2.74ms. 해당 장면 반복 효과 확인·후속 자산 연구 기준 통과. 표시 변경은 방향 불일치, 시각·기능·전체추적20ms 목표는 미수용.

[3쌍 검증 보고서](reports/operations/npc-palette64-client-highcap-three-pairs-20261105.md)
<!-- NPC_CPU_CURRENT_END -->

문서 갱신:2026-11-04

근접 화면에서 자체 P3D 프록시의 기본 이동·분절 팔다리 출력을 확인하고 실제 PresentMon 측정을 수행했다. X/Z source 이동차각1m를 교정했고,반대 끝 자세와 연속 자세 변화를 관찰했다. 자연스러운 보행·접지·광학 cadence의최종 수용은 아니다.

리테일 오프라인 fresh2회,1280×720,VSync1. 총11개15초창 중전면손실3개 제외·Steam 알림1개 보조. primary는빈 장면3,100개1,1000개3창이다. Present p95:빈 장면26.347–26.614ms,100개25.793ms,1000개29.800–30.894ms. 표시 p95는20.081–20.090ms로약50회/초를 유지했다. GPU Busy p50은빈 장면5.716–6.643ms,100개5.874ms,1000개7.441–7.608ms다.

**최종 수용 아님.** 개체당72삼각형/5본의로컬 프록시이며AI·물리·전투·네트워크는없다. 무제한/60FPS와20ms Present p95를 입증하지않았다. 다음단일작업은전용 profile의VSync를끈0/100/1000 처리량비교다. 현재시험은종료했으며별도 애니메이션 세션과기존NPC 배포는변경하지않았다.

[상세 보고서](reports/operations/npc-horde-close-present-20261104.md) · [근거](evidence/npc-horde-close-present-20261104.txt) · [JSON](api/public/npc-horde-close-present.json)

[직전 통합 상태](reports/history/20261104-before-close-present-README.md) · [AI 검토](AI_REVIEW.md) · [검증 정책](reports/operations/npc-validation-policy-20261103.md)

기존 상세 NPC 후보의 검증 이력은 직전 상태에 보존했다. 이번 프록시는 별도 자산 계열이며 기존 결과의 기능 복구·성능 개선으로 집계하지 않는다. 기존 서버/Workshop 배포는 변경하지 않았다. 시험 PBO 자동 서명·검증 성공,개인 키 배포0개.
