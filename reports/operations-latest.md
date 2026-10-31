# 5번 낙하 복구 정적 재현: 착지 거부·예산 소진과 안전 경로 분리

작성: 2026-10-31T14:54:44.915530+09:00

## 1. 작업 목표

실패한64명 시험 runtime-20261031-143044의5번 generation1을 기록된 포즈로 재현한다. 착지 공간 거부·수평 분리 예산 소진·실제 안전 경로의 존재를 구분한다. 시간/거리 한도 상향이나 world 검사 생략을 수정안으로 채택하지 않는다.

## 2. 실제 수행

99.815/200.168/299.197초 같은 호출의4관절 MS·root·heading·lift·idleOffset·tested 지점을 선택했다. nativeBefore/After와updatesBefore/After가 같음을 확인했다. 기존 격리 compile/smoke 환경에만 frozen body를 추가하고 원래 SolveContactsInternal의 bone getter 한 곳만 TablePose로 대체했다. JointWorldPoint도 같은 기록을 사용한다. 현재 B93D와 원본453D/부분0C6E를 로딩했다. 실시간 AI·플레이어·peer·애니메이션 변화는 없다.

3개 정적 엔진 프로브를 실행하고 소유 프로세스 종료를 확인했다. 각195행이다. 첫 chain 진단에서 초기화하지 않은 travel/accepted가 case 사이 누적되는 오류를 찾았다. 첫 원시/코드를 보존하고 그 chain 거리·횟수/clear 판정을 제외했다. 두 번째는 해당 진단 변수만 명시 초기화했다. 세 번째는 기존6m 범위 안에서 같은 직선 검사를 연장했다(단계 최대.03m,최대220회,실제 누계6m). runtime budget은 바꾸지 않았다. 등록 fixture의 실제 native collision box도2행 기록했다. 새 클라이언트·PBO·후킹·영상은 없다.

## 3. 측정 결과

- 실제 포즈3개 모두 원래 FIXTURE_VOLUME 착지 거부를 재현했다.
- 원래 소진 예산6.00022/6m에서는48/48 작은 이동이 SEPARATION_DISTANCE_LIMIT로 거부됐다.
- 같은 포즈의 fresh-budget 진단 대조군에서는21/48 이동이 기존7probe/world 검사에 통과했다. 나머지24개는 DEEPER_FIXTURE,3개는 NEW_WORLD_CONTACT다.
- 첫걸음·소진예산·fresh예산·착지 행은3실행에서 완전히 같았다. 첫 chain 누계는 무효이며 이후 초기화한 행만 사용한다.
- 짧은30step 검사에서는 완전한 정적 끝점을 찾지 못했다. 기존6m 범위 검사에서는 각 포즈마다2방향,총6개 끝점을 발견했다. 발견한 경로는1.18066m/39step 또는1.27921m/43step이다.
- 매 edge는 원래 비심화·관절·world 검사를 통과했고, 끝점은 원래 LandingSpace·SolveContacts·정적GroundRoute를 통과했다. 같은 실행 안에서 선택 방향을 순차 검사한 결과이며 별도의 전체 path 재실행이나 live motor 성공은 아니다.

새8/64 기능 회귀·p50/p95/p99·실제 화면 cadence·64비용은 미실행/미측정이다. 앞선63/64·95/96복귀·진입/출구 계약 실패를 변경하지 않는다.20ms는 미달성 목표다.

## 4. 원인 분석

확인: 초기 실제 지지 상실과 별개로,후반부에는 실제 착지 체적 조건이 거부하고 수평 분리 guard도 예산 소진으로 이동을 막았다. 포즈/기하를 고정하면 기존 검사 안에서 작은 이동과 완전한 정적 끝점이 존재한다. 따라서 물리적으로 어떤 안전 경로도 없다는 설명은 이 제한된 표본에서 지지되지 않는다.

제한: fresh-budget/외부 선택 방향은 진단 대조군이다. 실제 기본/정련 planner가 그 방향을 골라 변화하는 포즈와 peer 속에서 실행한다는 증거가 아니다. 본 실험으로 budget을 무한 reset하거나 기본 한도를 늘려도 안전하다고 주장하지 않는다. 현재 full endpoint는1.1m 정적 계획 inflation까지 포함하므로1.18m를 순수 착지만을 위한 최소 거리로 부르지 않는다. 텍스트 포즈는 반올림된 값이고 peer/플레이어가 없다.

이전 Fast 원인 귀속도 미확정이다. 실제5번의100~299초 정체 구간에는 Fast 사용0·기존10ray/solve였지만 초기 이동에 미친 경로 의존성은 배제하지 못했다. 정상 낙하는 계속 허용하고 미복귀만 별도로 해결한다. 첫 진단 누계 오류를 보존했고 잘못된 chain 수치를 성공 근거로 사용하지 않는다.

## 5. 검증 수준

동일 입력 착지 거부·소진 guard 재현: 진단 성공. 수정 후 독립 baseline/budget 행 일치: 제한적 반복 검증. 기존 검사 안의6개 정적 끝점 발견: 한정 정적 검증. live planner 선택/낙하→착지→FOLLOW·64회귀·성능·시각·최종 수용: 미완료. 실제 NPC 수정은 아직 없다.

## 6. 변경 사항

별도 진단 미션·stage/분석·보고 파생물만 추가했다. 원래 live 미션/Source/PBO/운영 서버/원시 기록 보존.3개 기존 PBO 서명 재검증 성공,새 PBO·재서명 없음. frozen solver의 getter 대체는 진단 subclass에만 있으며 배포 알고리즘 변경이 아니다. 개인키를 공개/복사하지 않았다.

## 7. 원시 증거

원 입력 runtime-20261031-143044 sequence198/360/457. 프로브 compile-7628ead671ed4860817bb6f21569bbf1,compile-34151ebafa1a46bd8707f99f3e1db153,compile-117b1077f8d14049818cebec971a189b. plan/counter-correction/corridor-plan/results,각engine-compile/init/195행TSV/fixture geometry/원로그를 연결한다. 저장 전 해시·JSON·파일 안정성·소유 서버 종료를 확인했다. 공개 선정본은 원시전체나64수용 근거가 아니다.

## 8. 잔여 문제

다음은 기록 입력에서 기존 기본/정련 PlanPressureEscape 선택과 full Fall→Landing 전이를 확인하는 일이다. 그 뒤 실제 수직/기하 진전·검증된 끝점·유한 lease를 조건으로 하는 제한적 복구 갱신을 검토한다. 도착 가능성이 있는 정적 경로를 live 구현 성공으로 바꾸지 않는다.35번 진입 정렬 실패와12번 출구 handoff 실패도 유지한다. 기능 실패가 남아 있어64성능 OFF/FAST 반복 배치는 보류한다. 애니메이션 표시 경로 연구와 별개다.

선정 공개 본문. 로컬 보고서 SHA-256: `094ed8845df0be4759403c7494c28217fb964a331dc571ecbb84c581c29a5a30`. 원시전체가 아니다.
