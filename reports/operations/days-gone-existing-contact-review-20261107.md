# Days Gone 기존 접촉·지지·밀침·애니메이션 비용 자료 검토

## 1. 작업 목표
새 실행 없이 기존 Days Gone 애니메이션·접촉·지지·밀침·CPU 연구의 실제 확보 범위와 DayZ 활용 가치를 재검토한다. 현재64개체 최적화 주력을 다른 엔진 전체 분석으로 바꾸지 않는다.

## 2. 실제 수행
2026-10-21/25 정적·런타임 요약, contact-support-active2 및 source-causal 기록,2026-11-02 CPU 스택 체크포인트를 읽었다. 첫 paused capture와 실패한 activity 대기 뒤의 active2 자료를 구분했다. 게임·REA 세션·후킹·새 PBO·영상·대용량 로그를 만들지 않았다. 이번 결과는 기존 파생 분석 검토이며 원시 약73MB/45MB 로그를 재분석했다고 주장하지 않는다.

## 3. 측정 결과
기존 active2는119.921초/2246스냅샷/15개체 포인터를 관찰했다. 낙하진입275회 중273회는 앞1초 안에 지지상실 관찰,226회는 앞250ms 내 peer접촉 관찰. 등반·낙하·착지·재등반 후보흐름61개는 완전한 인과 검증61회가 아니다.
별도35.140초 source-causal에는 peer기원 힘의 큐→속도 반영 연결3건, 같은edge-cache origin의 복수진입896묶음(최대5개체), 복수 새등반상태12묶음이 기록돼 있다. 입력힘×측정역질량과 실제속도증분 일치 사례는 있지만 전체충돌해법 복구는 아니다. 훅횟수와벽시계율은 비용값이 아니다.
기존 CPU ETL8.180409초/가중표본9756에서 본체미분류60.353%,PhysX/APEX말단5.525%였다. 이 물리비율은NPC물리 전체나지지순수비용이 아니다. 애니메이션비용은 분리실패. 지붕장면은 등록300중생존20/사망상태280,3440×1440/VSyncON이므로 DayZ64와직접비율비교 불가.

## 4. 원인 분석
확인된 경로: 접촉레코드의 body/개체ID·법선 판독→저장된support판정·air history 갱신. 검사한support predicate 자체에는 새geometry query가 없다. 이것은 contact생성단계나다른지지코드에도 query가 없다는 뜻이 아니다.
정적 batch: member50개 단위 job,contact최소chunk200 및후속wait관계. chunk50은프레임당50개만갱신한다는의미가아니다.
정적edge projection: 저장된기하에 개체별접근위치를투영하며 해당함수내world query/peer예약 없음. 전체시스템예약부재는미입증.
애니메이션: SharedAnimSkeletalMeshComponent/ActiveList/FlatList 메타데이터존재만확인. 현장면실행·공유율·LOD·포즈평가/스키닝비용미확인.
최초낮은접근기록만읽으면밀침과실시간자료를누락한다. 반대로후속3건을근거로힘생성식·마찰·질량·모든밀침과낙하의인과관계를완전복구했다고하면안된다.

## 5. 검증 수준
기존자료 재검토완료. 이번새런타임검증0,신규최적화0. DaysGone성능원리완전복구·DayZ적용성능·기능·시각수용없음. 확보된static/runtime/temporalassociation/unknown을분리한다.

## 6. 변경 사항
보고서만추가. 게임코드·PBO·서명·실행기·공개실험데이터변경없음. 새서명대상없음. 원시자료보존. 현재주력64비용측정결과를이참고검토로대체하지않는다.

## 7. 원시 증거
horde-solver-cost-drop-clearance-2026-10-21.json,contact-support-implementation-readiness-2026-10-25.json,contact-support-active2-20261025/contact-support-analysis.json,contact-source-causal-20261025/source-causal-analysis.json와2026-11-02 DaysGone stack checkpoint를입력해시로연결한다. 기존source-causal limit에는동일edgeorigin≠동일손접촉,peer힘반영≠뒤낙하단독원인,훅오버헤드≠성능결과가명시돼있다. 공개본은선정요약과해시만포함한다.

## 8. 잔여 문제
가치있는후속질문은접촉생성→support predicate→속도반영→애니메이션평가의비용소유자를기존ETL/정적자료에서분리할수있는가이다. 개체별foot ray를같이돌리는지보다동일접촉결과를여러소비자가읽는구조가핵심참고다. DayZ AnimalBase 파트에서동등한native contact생산/판정인터페이스를쓸수있는지는별도미확인이며DaysGone분석만으로제공되지않는다.
추가조사는①기존자산/세션재사용②하나의소유자연결③기존CPU표본에실제겹치는지의순서로한정할가치가있다. 연결되지않으면새게임/후킹을반복하지않는다. 이미실패한SupportFast단순ray축소를새아이디어로재시작하지않는다. 전체DaysGone재분석과다른엔진부품이식은이번검토에서권장하지않는다.
