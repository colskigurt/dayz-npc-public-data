# 정상 FOLLOW 접촉 작업 재대조

## 1. 작업 목표
새 미세후보를 구현하기 전에64개체 추가 접촉 검사가 실패 재시도인지 정상 FOLLOW 작업인지 기존 자료로 구분한다.
## 2. 실제 수행
8/64 진단의 기존 약30~60초 trace에서 개체별 누적 카운터 차이를 재계산했다. 위상·영역이 같은 구간과 전환 구간을 분리했다. 재현 스크립트로 저장 결과와 정확 일치함을 확인했다. 새 게임·PBO·영상·덤프는 없다. 기존 비용coverage 연구와 같은 분류 방법을 최신 진단 자료에 적용한 교차 확인이며 새로운 독립 성능 반복이 아니다.
## 3. 측정 결과
64개체 선택 구간 전체70347solve/1024failure. 안정 FOLLOW/ground58105solve/607failure(1.0447%),physics callback28276회로solve/callback2.055다. 8개체같은구분2110solve/5failure,callback1457회로1.448이다.64ground의footRays581050은solve당10회다. 표본경계 때문에 전체 EnProfiler70704호출과 동일 범위가아니다. p50/p95/p99 신규 없음.
## 4. 원인 분석
대부분 실패가아닌 정상 판정에서 검사가 반복됨을 확인했다. 실패 backoff만 주력으로 삼을 근거가 부족하다. Source에서DriveGround와Displace가 각기 다른 이동·접촉 구간을 검사하며FaceEvidence 등 다른호출자도 있다. 전체solve에서Drive를빼Displace정확호출수라고 할수없다. 기존SupportSweepFast/캐시/물리호스트/부분골격 연구의 기각 판정은유지한다. SDK의두Bullet선언에는배치인자나사용자scratch인자가없다. RV대안은동일필터·형상판정이라는증거가없어즉시치환하지않는다.
## 5. 검증 수준
기존기록재현·소유경로정합성진단성공. 성능개선·기능회귀·시각품질은신규판정없음.20ms달성아님.
## 6. 변경 사항
기존진단폴더안의파생analysis/reproduce/decision만추가. 운영소스·PBO·안전판정·실행기불변. 서명대상없음. 시험프로세스없음확인.
## 7. 원시 증거
ANIM-npc-owner-diag-eight1과ANIM-npc-owner-diag-sixtyfour2의gameplay-eight.tsv,기존함수배열과현재DGBNativeGround/DGBNativeCrowd/DayZPhysics선언. 파생분석은원시로그를덮어쓰지않으며재현결과도독립반복으로세지않았다.
## 8. 잔여 문제
정확한Displace호출수·성공률·상위소유자별시간은현재top60호출표만으로분리되지않는다. 다음한정질문은정상Displace와DriveGround의중복작업분리이며,결과를얻기전검사통합·시뮬레이션LOD·실패복구개발을시작하지않는다. 이전소규모진입캐시의gate미달을무시하고긴64회귀를반복하지않는다.
