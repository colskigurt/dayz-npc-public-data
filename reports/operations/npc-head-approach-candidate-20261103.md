## 1. 작업 목표
기존64개체 실패에서 방향 정렬과 접촉 복구를 구분하고 국소 수정 후보를 선정한다. 바닐라 추가조사와 새게임실행은 보류했다.

## 2. 실제 수행
닫힌 gameplay/state/replay/frame/contact/pose-use를 재분석했다. replay stage1 경계에서 왼쪽 표본을 다음표본까지 유지하는 시간추정, contact Director.Now를 server-frame gameTickTime/seconds로 선형대조했다. 새독립실행0회다. 기존 pose-core-v2가 !Active를 요구함을 확인했다. 격리 대체모듈에 기본OFF pose-core-approach.flag를 추가했다. OpenTraversal·지면CLIMB접근·출발영역일치·PendingGoal없음·정렬전·전투/사망없음일때만 기존자세복구를 허용한다.

## 3. 측정 결과
원래64명49/64실패유지. 높은발판미도달12,재등반만부족3. stage1은90.1818~137.932초,47.7502초다. 지면체류추정평균 미도달12명34.614917초/도달52명1.587946초. 최대표본간격0.375초다. 미도달집단의지지상실FALL13건,등반끝실패FALL1건. FOLLOW Head/컨테이너 로그40개/도달집단3개이며25/40개가직전표본의지면CLIMB연결과겹쳤다. 개체당0.5게임초제한로그이므로전체호출량이나CPU비용이아니다. p50/p95/p99·화면·CPU/GPU/GC는미측정이고20ms는목표다.

## 4. 원인 분석
사실: MoveTo후SolveContacts실패시위치/heading이롤백되고이후군집/복구이동이가능하다. ALIGN_HEADING을개체정지로해석하지않는다. 기존Head검사는있지만!Active가접근도제외한다. 가설: 이범위가재접근지연에기여한다. 직전표본만으로동일callback의guard탈락은확정못한다. 회전속도조정·충돌삭제·정상낙하금지·기한연장은제외한다.

## 5. 검증 수준
진단부분성공. 후보빌드/서명/packed script대조성공. 실제엔진컴파일·fixture·8/64회귀·시각·성능·최종수용미실행. 이전회귀성공을후보로승계하지않는다.

## 6. 변경 사항
pose-core-approach-candidate1의동일모듈대체본만생성했다. 기존nf_pose_core_recovery와동시로딩금지. proposedHeading기본값처리이후물리검사본문은원본과동일하고7spheres·반경·support·벽·군집·예약검사를유지했다. SupportedEscapeTurn의Active차단은그대로다. 원본소스/기존PBO/운영서버/Steam/자산/로그미변경. 개인키미포함. 빌드ID는공개JSON에연결한다.

## 7. 원시 증거
high-window-analysis,alignment-episode-analysis,high-contact-correlation,head-approach-eligibility-analysis,reapproach-decision과후보plan/build/sign/packed/source를로컬해시로연결했다. 원시전체는보존하고공개는집계만제공한다. 같은기록재분석은독립반복이아니다.

## 8. 잔여 문제
기존동결자세에서선택된CLIMB접근과후보OFF/ON을비교한다. 플래그OFF/Active없음/정렬중/동작중/전투/사망/목표변경/다른영역은기존판정유지. 안전한추가진전이없거나guard누출이면기각하고8/64확대하지않는다. 통과할때만원래기한·밀침·낙하·착지·FOLLOW·재등반계약으로8→64확인한다. 접근/복구진동과추가검사비용이위험이다. 전체골미완료.

로컬checkpointSHA256: 7768199d222a66b6bd080db38fc95598a6e8cd36fb356e367f1b3f26248c3ebf
