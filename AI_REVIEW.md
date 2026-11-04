# DayZ 대량 호드 표현 경로 검토

<!-- NPC_CPU_CURRENT_BEGIN -->
## 현재 CPU 최적화 상태

정지Run·VSync0·500상한3쌍에서5파트 CPU Busy/Present p95 감소 방향 재현. 쌍별중앙값 약16.8/16.7%. 해당 장면의 효과이며 전체 게임/시각 채택은 미완료. [실측](reports/operations/npc-palette64-client-highcap-three-pairs-20261105.md)

## 현재 준비 — 신규 측정 없음

14클립을 한 서버에서 순차 대조하는 소스 준비. 5쌍·401본×4벡터와 루트/콜백 시간 요약. 컴파일·실행·성능 측정 없음. Native65클립은 편집기 파일 접근 승인 대기. 실제 상태 전환·이벤트·시각 검증은 별도.

[시험 준비](reports/operations/npc-palette64-fullclip-pose-fixture-staged-20261105.md)
<!-- NPC_CPU_CURRENT_END -->

근접 화면에서 자체 P3D 프록시의 기본 이동·분절 팔다리 출력을 확인하고 실제 PresentMon 측정을 수행했다. X/Z source 이동차각1m를 교정했고,반대 끝 자세와 연속 자세 변화를 관찰했다. 자연스러운 보행·접지·광학 cadence의최종 수용은 아니다.

리테일 오프라인 fresh2회,1280×720,VSync1. 총11개15초창 중전면손실3개 제외·Steam 알림1개 보조. primary는빈 장면3,100개1,1000개3창이다. Present p95:빈 장면26.347–26.614ms,100개25.793ms,1000개29.800–30.894ms. 표시 p95는20.081–20.090ms로약50회/초를 유지했다. GPU Busy p50은빈 장면5.716–6.643ms,100개5.874ms,1000개7.441–7.608ms다.

**최종 수용 아님.** 개체당72삼각형/5본의로컬 프록시이며AI·물리·전투·네트워크는없다. 무제한/60FPS와20ms Present p95를 입증하지않았다. 다음단일작업은전용 profile의VSync를끈0/100/1000 처리량비교다. 현재시험은종료했으며별도 애니메이션 세션과기존NPC 배포는변경하지않았다.

[상세 보고서](reports/operations/npc-horde-close-present-20261104.md) · [근거](evidence/npc-horde-close-present-20261104.txt) · [JSON](api/public/npc-horde-close-present.json)

검토 질문:VSync를해제해도0/100/1000 비용/표시가유지되는가;5본/72삼각형에서품질을올릴예산이얼마인가;전면손실창 제외와CPU/GPU전체비용의해석이적절한가. 실제Present 호출간격과Display간격,네이티브 본변환과광학연속품질을구분한다.

[이전 AI 검토 상태](reports/history/20261104-before-close-present-AI_REVIEW.md) · [기존 AI 소스 분석](reports/operations/npc-horde-ai-source-review-20261104.md)
