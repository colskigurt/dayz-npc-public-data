# 접촉 빈도 후보 사전검사 — 33ms 제안 철회

## 1. 작업 목표
군중Update 캐시의 주력 확대 보류 후,후속 밀침/지지/월드 질의의 변경 가능한 범위를 기존 자료로 확인한다. 정상 밀침·낙하·착지·추적·재등반을 보존하며 무익한 빈도 조정 시험을 피한다.

## 2. 실제 수행
현행 SolveContactsInternal/Drive/Displace와 기존 query-control,SupportSweepFast,Scratch,StaticBounds,Drive→Displace 결합 보고서를 대조했다. 먼저 지상 접촉을33ms 단위로 모으는 후보를 사용자에게 제시했으나,구현 전에 기존 최신64 로그의 physicsUpdates/Displace 카운터로 전제부터 검사했다. 물리 콜백이 이미약20Hz임을 확인하고 제안·선택 질문을 철회했다. 선택 답변이 오더라도 철회한33ms 후보의 구현 승인으로 취급하지 않는다.

## 3. 측정 결과
같은 기존 ANIM-crowd-live-attribution1 게임10~60초 자료의64개체 모두 물리 콜백 빈도 **19.996976/개체게임초**. 합계 callback 63488,접촉Solve 121908,Displace호출 51619,그중Solve 50833. Displace의Solve 수는 같은 표본 범위 전체Solve의 **41.6978%**이며 CPU비용 비중이 아니다.1callback에서항상2solve가발생한다는뜻도아니다.
새 게임/클라이언트/계측/PBO0. 새 Present p50/p95/p99·시각 판정·성능 개선0. 이 재분석을 독립 반복으로 세지 않는다.

## 4. 원인 분석
확인: 평균50ms 기반 스트림에33ms 묶음 조건을 더해 호출이 줄어든다는 전제가 성립하지 않는다. 개별callback간격 히스토그램은 없어 모든간격이정확히50ms라고확정하지않지만,현재자료는33ms최적화의근거가아니다. 이 물리callback빈도를엔진전체애니메이션평가/렌더프레임빈도로해석하지않는다.

| 검토 지점 | 확인된 경계 | 현재 결정 |
|---|---|---|
| native query 내부 scratch/batch | 설치선언에scratch/batch인자미확보 | 스크립트출력배열재사용으로native할당제거주장금지 |
| Drive→Displace 결과 캐시/병합 | 입력42쌍완전일치0,중간Contacts와부분성공rollback의존 | 단일endpoint로바꾸지않음 |
| SupportSweepFast | 현재64 ray감소에도전체비용수용실패;기존bound오차최대약8mm | 1mm허용을이후보로확대하지않음 |
| StaticBounds epoch | StaticMap metadata용,실제8hit0 | 발지지중복제거와동일시하지않음 |
| no-op 접촉 조기탈락 | 기존8 낙관적전체Solve수0.89~1.08%규모 | 같은시험재개하지않음 |
| 33ms 접촉묶음 | 현재nativecallback평균20Hz | 구현전철회 |

미확인: 더긴지연의실제반응성/안전성/전체CPU효과. 더긴지연으로몰래수정해새후보로실행하지않는다. 이미검사하는중간경로를없애야만생기는절감은기능보존성공으로보고하지않는다.

## 5. 검증 수준
기존로그빈도·호출소유진단성공. 구현전후보철회. 기능회귀·시각·성능개선·최종수용미실행. 기존1mm발지지오차선택과별개며,새허용범위를추정하지않았다.20ms p95목표미달성유지.

## 6. 변경 사항
기존work-reduction-review1 안에재분석Python·작은JSON·보고서만추가. 원본/PBO/모델/물리검사/갱신주기/행동/운영서버불변. 새서명대상없음. 질문을근거확인전에제시한순서오류는기록하고,결과를알면서33ms시험을시작하지않았다.

## 7. 원시 증거
ANIM-contact-cadence-feasibility1은ANIM-crowd-live-attribution1의재분석. 원본gameplay-eight.tsv/ground-progress.tsv/part-command-clock.tsv 해시를전후대조했다. contact-cadence-feasibility1.json,check-contact-cadence1.py와기존query-control/combined-proposal/SupportFast/StaticBounds 보고서를연결한다. 기존로그·보고서덮어쓰기없음.

## 8. 잔여 문제
물리질의의큰실행량은남아있고,현재검토범위에서즉시적용할새안전후보는확정하지못했다. 실제수정가능한native API 또는안전검사동등성의새근거없이추가파라미터시험을하지않는다. 다음선정에서는질의대체의지원계약과중간접촉경로보존을먼저입증해야한다. 현재까지의작은함수개선·호출수재분석을64전체최적화성공으로대체하지않는다. 애니메이션시각품질은기존사용자14동작평가와신규64판정을구분해유지한다.
