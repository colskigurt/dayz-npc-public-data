## 1. 작업 목표

64명 ON1에서 장기 FALL 상태였던24번의 기존 복구 경로가 가까운 착지점으로 이동하지 못한 이유를 좁힌다. 포즈 기반 몸통 검사라는 기존 연구를 필요한 범위에만 재사용하고, 실제 안전 검사·정상 낙하·착지·추적·재등반을 유지하는 격리 후보를 검증한다.

## 2. 실제 수행

기존 실제 escape trace1048행을 대조했다. 뒤쪽 첫3cm는 fixed core1의 NEW_WORLD_CONTACT 거부였다. 당시 보수적box clearance는 양수였지만 native 시작 접촉이 보고돼 원래 비겹침/이탈 조건에서 막혔다. 이 값만으로 native 침투가 없다고 하지 않는다.

기록된4본·root·heading·lift를 frozen body에 넣고 원래 복구와 pose-derived3body spheres를 비교했다. 반경0.23m·원래4관절·월드 스윕·보수적 비심화·peer/예약·terrain lift·원래 LandingSpace는 유지했다. 후보는 FALL·압력 낙하·request8·4관절·실제 지면0.2m 이내·동작/진입정렬 없음에만 적격이고 기존terrain owner는 위임한다.

기본OFF 별도PBO를 빌드·자동 서명·서명 검증·packed source byte 대조했다. 실제 PBO를 사용한 frozen 재생에서도 긍정/음성 대조를 확인했다. 이후 기존8명 고정입력 씬에11번째 패키지를 추가하고 profile flag로만 켰다. 같은 입력과 정상 기능 기준을 유지하고 실제 사용 카운터를 추가했다. 새영상/native hook은 없다.

## 3. 측정 결과

frozen 원래 fixed core: 첫 이동0/8, motor100호출에서0걸음·착지 거부 유지. pose core: 첫 이동2/8, motor1걸음 약0.0302734m 후 원래 LandingSpace와 전체 포즈 검사 통과다. 이는 수평 synthetic motor이며 실제 중력·착지 상태 인계를 실행한 시험이 아니다.

적격 외12조건은 원래 반환값과 같고 후보 사용0이었다. 실제 월드 스윕이 벽을 검출한4면 모두 후보가 활성화된 상태에서 진입을 거부했다. prototype과 실제PBO에서 같은 frozen 입력을 재생했으며 같은포즈 재생을 독립적인 동적복구 반복으로 합산하지 않는다. 패키지 defaultOFF는 read=true/enabled=false/uses0/delegations1로 직접 확인했다.

8명 실제 시험: 유효 종료8/8, 전체 이력8/8, FALL9/LAND→FOLLOW9. 기능 판정 통과. 상태 체인/세대/종료 정합성 오류는 0/0/0이다.

실제 후보 사용 개체0/8·적격 검사0회, 원래 경로 위임149회다. 사용0이면 이번8명 결과는 동적 효능의 증거가 아니다. 사용 횟수는 실패한 적격 검사도 포함하므로 성공한 복구 횟수와 다르다.

서버Mission dt p50/p95/p99=16.0/31.0/32.0ms, 명령시간/미션시간=0.999962. client Present·CPU busy·GPU·할당/GC·실제 표시 품질은 미측정이다. 기존10PBO8명 실행과 matched 성능 비교가 아니며20ms p95는 목표다.

## 4. 원인 분석

확인: 캡처한 FALL 포즈에서는 일반4관절 solver가 통과해도 root 고정 몸통을 추가하는 복구 경로가 이동을 거부했다. 같은 swept/non-deepening 조건에서 몸통 중심만 포즈에 맞추면3cm 이동 후 원래 착지 검사가 통과했다. 기존 FOLLOW-only 포즈 복구는 FALL에서 위임하므로 이 사례에 적용되지 않았다.

가설/미확인: 동적 포즈 변화·군집 접촉에서도 같은 회복이 안정적인지,64명 정렬 실패와 비용을 개선하는지는 아직 모른다. native/AABB의 시작 겹침 의미 차이가 모든 접촉에서 해소된 것은 아니다. 이 후보는 성능 최적화나 애니메이션 시계 수정이 아니다. 근지면 floor2회 추가 비용은 미측정이다.

효과 부족 연구와 구분: 기존16방향/1cm 이동 후보는 이전 다른포즈에서 효과 부족이었다. 이번에는 방향이나 걸음 길이를 바꾸지 않았고 복구 볼륨의 공간 계약을 비교했다. 착지true 강제·충돌 생략·반경 축소·순간이동·실패 라벨/시간 제한 완화는 하지 않았다.

## 5. 검증 수준

캡처 원인 구분·frozen motor/guard·실제 패키지/defaultOFF: 진단 성공.8명 단일 기능 회귀: 통과. 동적 복구 효능·64명 회귀·시각 품질·반복 성능·최종 수용: 미완료다. 서버는DONE 후 하네스가 소유 정리하며 정상quit 검증은 아니다.

## 6. 변경 사항

새 nf_near_ground_fall PBO SHA256=1A1928F160D0852659B4CB2E8AF87295448FB3B3BB075FFD039A703BB1FB461D. 자동 서명·DSCheckSignatures·packed script byte 검증 성공, 실제 World/Mission compile과 패키지 재생 성공이다.8명 런처에서11개 패키지 해시/서명 재검증 성공. 원본10PBO·원본 클립·그래프·운영 서버·Steam·개인키는 변경하지 않았다. 새 모듈은 기본OFF이며 profile flag 제거 또는 모듈 제외로 복귀한다.

준비 오류도 보존했다. comparison1은 OnUpdate 인자명이 SDK timeslice와 달라 컴파일 가드로 거부됐다. guards1은 indexed coordinate의 숫자 빼기 표기에서 컴파일 가드가 거부했고, 별도guards2에서 연산자 간격만 수정했다. rootUnchanged 표현식은 bool 문자열 대신1로 출력돼 파서가 typed 표현의 두 표기를 구분했다.8명 시작 도우미의 attribute 괄호 오류는 실행 전PowerShell parser로 발견해 수정했다. 이 준비 실패를 후보 실행 성공으로 세지 않는다.

## 7. 원시 증거

network-gameplay/slot24-fall-pose-comparison2, slot24-fall-pose-guards2, slot24-fall-pose-packaged1, near-ground-fall-candidate1, near-ground-fall-eight-on1을 연결한다. prototype/패키지·미션·상태/사용/명령/프레임·종료launch·빌드/서명/packed proof와 준비 실패comparison1/guards1을 로컬 해시로 보존한다. 공개는 집계만 제공하고 포즈/좌표·절대경로·프로세스·원시로그·게임 자산·개인키는 제외한다. 진행 중인 후속 실행 로그는 이 완료 묶음에 포함하지 않는다.

## 8. 잔여 문제

같은11개 서명 패키지와 실제 사용 계측으로64명 기능 시험을 진행한다. 빠른 지지 검사는OFF로 유지하고 원래 판정 기준을 보존한다.

64명 진입은8명 원래 기능 판정이 통과해야 허용한다. 동적 후보 사용이 없다면 안전성만 보고하고 실제 사용/회복 에피소드를 확보해야 한다. 전체 고정입력 종료·정상 낙하 복귀·재등반·미해결 정체를 유지하며64명 모두가 좁은 지붕에 동시에 남아 있어야 한다고 요구하지 않는다. 실패하면 원시 실패를 보존하고 국소 원인으로 돌아간다. 물리 비용·네트워크 표시 품질·20ms 목표는 독립 과제로 유지한다.

로컬checkpointSHA256: 113f492301813761cec14602af801d27684a355987793a182f6960312143f983
